// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once
#include "EmptyPlacement.h"
#include "Node.h"
#include "Optional.h"

namespace mxml {
namespace dom {

class Articulation : public EmptyPlacement {
public:
    enum class Type {
        Accent,
        BreathMark,
        Caesura,
        DetachedLegato,
        Doit,
        Falloff,
        Plop,
        Scoop,
        Spiccato,
        Staccatissimo,
        Staccato,
        Stress,
        StrongAccent,
        Tenuto,
        Unstress
    };
    
public:
    Articulation() = default;
    Articulation(Type type) : _type(type) {}
    
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
