// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once
#include "Node.h"
#include "Optional.h"
#include "Pitch.h"

namespace mxml {
namespace dom {

class Rest : public Node {
public:
    Rest() : _displayStep(Pitch::Step::C), _displayOctave(4) {}
    Rest(Pitch::Step step, int octave)
    : _displayStep(step), _displayOctave(octave) {}
    
    const Optional<Pitch::Step>& displayStep() const {
        return _displayStep;
    }
    
    void setDisplayStep(Optional<Pitch::Step> s) {
        _displayStep = s;
    }
    
    const Optional<int>& displayOctave() const {
        return _displayOctave;
    }
    
    void setDisplayOctave(Optional<int> octave) {
        if (octave < 0)
            _displayOctave = 0;
        else if (octave > 9)
            _displayOctave = 9;
        else
            _displayOctave = octave;
    }
    
private:
    Optional<Pitch::Step> _displayStep;
    Optional<int> _displayOctave;
};

} // namespace dom
} // namespace mxml
