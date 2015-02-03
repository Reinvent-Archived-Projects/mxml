//  Created by Alejandro Isaza on 2014-05-05.
//  Copyright (c) 2014 Venture Media Labs. All rights reserved.

#pragma once
#include "Geometry.h"
#include "NoteGeometry.h"
#include "TieGeometry.h"

#include <mxml/dom/Note.h>

#include <map>
#include <memory>
#include <vector>


namespace mxml {

class TieGeometryFactory {
public:
    static const coord_t kTieSpacing;
    
public:
    explicit TieGeometryFactory(const Geometry& parentGeometry, const Metrics& metrics);
    
    std::vector<std::unique_ptr<TieGeometry>>&& buildTieGeometries(const std::vector<std::unique_ptr<Geometry>>& geometries);
    
private:
    void createGeometries(const std::vector<std::unique_ptr<Geometry>>& geometries);
    void createGeometriesFromNotes(const std::vector<std::unique_ptr<NoteGeometry>>& notes);
    void createGeometriesFromNotes(const std::vector<NoteGeometry*>& notes);
    void createGeometryFromNote(NoteGeometry* noteGeometry);
    std::unique_ptr<TieGeometry> buildTieGeometry(const NoteGeometry* startGeom, const NoteGeometry* stopGeom, const dom::Optional<dom::Placement>& placement);
    std::unique_ptr<TieGeometry> buildTieGeometryFromEdge(const NoteGeometry* stopGeom, const dom::Optional<dom::Placement>& placement);
    std::unique_ptr<TieGeometry> buildTieGeometryToEdge(const NoteGeometry* startGeom, const dom::Optional<dom::Placement>& placement);
    std::unique_ptr<TieGeometry> buildSlurGeometry(const NoteGeometry* startGeom, const NoteGeometry* stopGeom, const dom::Optional<dom::Placement>& placement);
    
private:
    const Geometry& _parentGeometry;
    const Metrics& _metrics;

    std::vector<std::unique_ptr<TieGeometry>> _tieGeometries;
    std::map<std::pair<int, int>, NoteGeometry*> _slurStartGeometries;
    
    typedef std::pair<int, const dom::Pitch*> PitchKey;
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
