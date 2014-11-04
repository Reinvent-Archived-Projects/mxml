//  Created by Alejandro Isaza on 2014-05-06.
//  Copyright (c) 2014 Venture Media Labs. All rights reserved.

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
    BeamGeometry(std::vector<ChordGeometry*> chords, const dom::Attributes& attributes);
    
    const std::vector<ChordGeometry*>& chords() const {
        return _chords;
    }
    const dom::Attributes& attributes() const {
        return _attributes;
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
    
    /** Get the tip of the stem for a chord. */
    Point stemTip(const ChordGeometry* chordGeom) const;
    
    void recomputeFrame();
    
private:
    void build();
    
private:
    std::vector<ChordGeometry*> _chords;
    const dom::Attributes& _attributes;

    dom::Placement _placement;
    Point _beamBegin;
    Point _beamEnd;
};

} // namespace mxml
