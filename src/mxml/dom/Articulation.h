//  Created by Alejandro Isaza on 2014-04-23.
//  Copyright (c) 2014 Venture Media Labs. All rights reserved.

#pragma once
#include "EmptyPlacement.h"
#include "Node.h"
#include "Optional.h"

namespace mxml {
namespace dom {

class Articulation : public EmptyPlacement {
public:
    enum Type {
        ACCENT,
        BREATH_MARK,
        CAESURA,
        DETACHED_LEGATO,
        DOIT,
        FALLOFF,
        PLOP,
        SCOOP,
        SPICCATO,
        STACCATISSIMO,
        STACCATO,
        STRESS,
        STRONG_ACCENT,
        TENUTO,
        UNSTRESS
    };
    
public:
    Articulation() {}
    
    Type type() const {
        return _type;
    }
    void setType(Type type) {
        _type = type;
    }
    
private:
    Type _type;
};

} // namespace dom
} // namespace mxml
