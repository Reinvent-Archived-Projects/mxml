// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once
#include "DirectionType.h"

namespace mxml {
namespace dom {

class Wedge : public DirectionType {
public:
    enum class Type {
        Crescendo,
        Diminuendo,
        Stop,
        Continue
    };
    
public:
    Wedge() : _type(), _number(1), _spread(15), _niente(false) {}
    
    bool span() const {
        return true;
    }
    
    Type type() const {
        return _type;
    }
    void setType(Type type) {
        _type = type;
    }
    
    int number() const {
        return _number;
    }
    void setNumber(int number) {
        _number = number;
    }
    
    float spread() const {
        return _spread;
    }
    void setSpread(float spread) {
        _spread = spread;
    }
    
    bool niente() const {
        return _niente;
    }
    void setNiente(bool niente) {
        _niente = niente;
    }
    
private:
    Type _type;
    int _number;
    float _spread;
    bool _niente;
};

} // namespace dom
} // namespace mxml
