// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once
#include "Node.h"
#include "Pitch.h"

namespace mxml {
namespace dom {

class Unpitched : public Node {
public:
    Unpitched() : _displayStep(Pitch::Step::C), _displayOctave(4) {}
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
