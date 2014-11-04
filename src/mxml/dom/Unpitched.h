//  Created by Alejandro Isaza on 2/17/2014.
//  Copyright (c) 2014 Venture Media Labs Inc. All rights reserved.

#pragma once
#include "Node.h"
#include "Pitch.h"

namespace mxml {
namespace dom {

class Unpitched : public Node {
public:
    Unpitched() : _displayStep(Pitch::STEP_C), _displayOctave(4) {}
    Unpitched(Pitch::Step step, int octave)
    : _displayStep(step), _displayOctave(octave) {}
    
    Pitch::Step displayStep() const {
        return _displayStep;
    }
    
    void setDisplayStep(Pitch::Step s) {
        _displayStep = s;
    }
    
    int displayOctave() const {
        return _displayOctave;
    }
    
    void setDisplayOctave(int octave) {
        if (octave < 0 || octave > 9)
            throw InvalidDataError("octave not in valid range");
        _displayOctave = octave;
    }
    
private:
    Pitch::Step _displayStep;
    int _displayOctave;
};

} // namespace dom
} // namespace mxml
