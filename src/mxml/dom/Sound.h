//  Created by Alejandro Isaza on 2014-05-14.
//  Copyright (c) 2014 Venture Media Labs. All rights reserved.

#pragma once
#include "Node.h"
#include "Optional.h"

namespace mxml {
namespace dom {

class Sound : public Node {
public:
    Sound() {}
    
    /**
     Tempo in quarter notes per minute.
     */
    const Optional<float>& tempo() const {
        return _tempo;
    }
    void setTempo(const Optional<float>& tempo) {
        _tempo = tempo;
    }
    
    /**
     Dynamics (or MIDI velocity) are expressed as a percentage of the default forte value (90 for MIDI 1.0).
     */
    const Optional<float>& dynamics() const {
        return _dynamics;
    }
    void setDynamics(const Optional<float>& dynamics) {
        _dynamics = dynamics;
    }
    
private:
    Optional<float> _tempo;
    Optional<float> _dynamics;
};

} // namespace dom
} // namespace mxml
