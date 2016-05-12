// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once
#include "Geometry.h"
#include "TieGeometry.h"

#include <mxml/dom/Note.h>

#include <map>
#include <memory>
#include <vector>


namespace mxml {

class ChordGeometry;
class NoteGeometry;
class Metrics;

class TieGeometryFactory {
public:
    constexpr static const coord_t kTieSpacing = 2;
    constexpr static const coord_t kSlurStemOffset = 10;
    
public:
    explicit TieGeometryFactory(const Geometry& parentGeometry, const Metrics& metrics);
    
    std::vector<std::unique_ptr<TieGeometry>>&& buildTieGeometries(const std::vector<std::unique_ptr<Geometry>>& geometries);
    
private:
    using SlurKey = std::pair<int, int>;
    using PitchKey = std::pair<int, const dom::Pitch*>;

    void createGeometries(const std::vector<std::unique_ptr<Geometry>>& geometries);
    void createGeometriesFromChord(ChordGeometry* chord);
    void createGeometryFromNote(NoteGeometry* noteGeometry);

    void buildTieGeometry(const PitchKey& key, NoteGeometry& noteGeometry, const dom::Tied& tie);
    std::unique_ptr<TieGeometry> buildTieGeometry(const NoteGeometry* startGeom, const NoteGeometry* stopGeom, const dom::Optional<dom::Placement>& placement);
    std::unique_ptr<TieGeometry> buildTieGeometryFromEdge(const NoteGeometry* stopGeom, const dom::Optional<dom::Placement>& placement);
    std::unique_ptr<TieGeometry> buildTieGeometryToEdge(const NoteGeometry* startGeom, const dom::Optional<dom::Placement>& placement);

    void buildSlurGeometry(const SlurKey& key, NoteGeometry& noteGeometry, const dom::Slur& slur);
    std::unique_ptr<TieGeometry> buildSlurGeometry(const NoteGeometry* startGeom, const NoteGeometry* stopGeom, const dom::Optional<dom::Placement>& placement);
    std::unique_ptr<TieGeometry> buildSlurGeometryFromEdge(const NoteGeometry* stop, const dom::Optional<dom::Placement>& placement);
    std::unique_ptr<TieGeometry> buildSlurGeometryToEdge(const NoteGeometry* start, const dom::Optional<dom::Placement>& placement);
    
private:
    const Geometry& _parentGeometry;
    const Metrics& _metrics;

    std::vector<std::unique_ptr<TieGeometry>> _tieGeometries;
    std::map<SlurKey, std::pair<const dom::Slur*, NoteGeometry*>> _slurStartGeometries;

    struct PitchComparator {
        bool operator()(const PitchKey& a, const PitchKey& b) const {
            if (a.first < b.first)
                return true;
            
            if (a.first > b.first)
                return false;
            
            if (a.second->step() < b.second->step())
                return true;
            
            if (a.second->step() > b.second->step())
                return false;
            
            return a.second->octave() < b.second->octave();
        }
    };
    std::map<PitchKey, std::pair<const dom::Tied*, NoteGeometry*>, PitchComparator> _tieStartGeometries;
};

} // namespace mxml
