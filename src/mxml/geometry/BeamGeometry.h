// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once
#include "Geometry.h"
#include "ChordGeometry.h"

namespace mxml {

class BeamGeometry : public Geometry {
public:
    static const coord_t kMinStem;
    static const coord_t kMaxSlope;
    
    static const coord_t kStemLineWidth;
    static const coord_t kBeamLineWidth;
    static const coord_t kBeamLineSpacing;
    static const coord_t kHookLength;
    
public:
    BeamGeometry(std::vector<ChordGeometry*> chords);
    
    const std::vector<ChordGeometry*>& chords() const {
        return _chords;
    }

    dom::Placement placement() const {
        return _placement;
    }
    const Point& beamBegin() const {
        return _beamBegin;
    }
    const Point& beamEnd() const {
        return _beamEnd;
    }

    coord_t slope() const;
    
    /** Get the tip of the stem for a chord. */
    Point stemTip(const ChordGeometry* chordGeom) const;
    
    void recomputeFrame();
    
private:
    void build();
    
private:
    std::vector<ChordGeometry*> _chords;

    dom::Placement _placement;
    Point _beamBegin;
    Point _beamEnd;
};

} // namespace mxml
