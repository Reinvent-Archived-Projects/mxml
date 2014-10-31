//  Created by Alejandro Isaza on 2014-03-25.
//  Copyright (c) 2014 Venture Media Labs. All rights reserved.

#pragma once
#include "Node.h"
#include <map>

namespace mxml {
namespace dom {

class Appearance : public Node {
public:
    enum LineType {
        LINE_BEAM,
        LINE_BRACKET,
        LINE_DASHESs,
        LINE_ENCLOSURE,
        LINE_ENDING,
        LINE_EXTEND,
        LINE_HEAVY_BARLINE,
        LINE_LEGER,
        LINE_LIGHT_BARLINE,
        LINE_OCTAVE_SHIFT,
        LINE_PEDAL,
        LINE_SLUR_MIDDLE,
        LINE_SLUR_TIP,
        LINE_STAFF,
        LINE_STEM,
        LINE_TIE_MIDDLE,
        LINE_TIE_TIP,
        LINE_TUPLET_BRACKET,
        LINE_WEDGE,
    };
    
    enum NoteType {
        NOTE_CUE,
        NOTE_GRACE,
        NOTE_LARGE,
    };
    
    enum DistanceType {
        DISTANCE_BEAM,
        DISTANCE_HYPHEN
    };
    
public:
    Appearance() {
        _lineWidths[LINE_STEM] = 0.9375;
        _lineWidths[LINE_BEAM] = 5.2083;
        _lineWidths[LINE_STAFF] = 0.9375;
        _lineWidths[LINE_LIGHT_BARLINE] = 1.875;
        _lineWidths[LINE_HEAVY_BARLINE] = 5;
        _lineWidths[LINE_LEGER] = 1.875;
        _lineWidths[LINE_ENDING] = 1.6667;
        _lineWidths[LINE_WEDGE] = 1.4583;
        _lineWidths[LINE_ENCLOSURE] = 0.9375;
        _lineWidths[LINE_TUPLET_BRACKET] = 0.9375;
        
        _noteSizes[NOTE_GRACE] = 65;
        _noteSizes[NOTE_CUE] = 65;
        
        _distances[DISTANCE_BEAM] = 8;
        _distances[DISTANCE_HYPHEN] = 60;
    }
    
    float lineWidth(LineType type) const {
        auto it = _lineWidths.find(type);
        if (it == _lineWidths.end())
            return 1;
        return it->second;
    }
    
    float noteSize(NoteType type) const {
        auto it = _noteSizes.find(type);
        if (it == _noteSizes.end())
            return 100;
        return it->second;
    }
    
    float distance(DistanceType type) const {
        auto it = _distances.find(type);
        if (it == _distances.end())
            return 1;
        return it->second;
    }
    
private:
    std::map<LineType, float> _lineWidths;
    std::map<NoteType, float> _noteSizes;
    std::map<DistanceType, float> _distances;
};

} // namespace dom
} // namespace mxml
