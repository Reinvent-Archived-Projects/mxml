//  Created by Alejandro Isaza on 2014-05-12.
//  Copyright (c) 2014 Venture Media Labs. All rights reserved.

#pragma once
#include "DirectionType.h"

namespace mxml {
namespace dom {

class Wedge : public DirectionType {
public:
    enum Type {
        TYPE_CRESCENDO,
        TYPE_DIMINUENDO,
        kStop,
        kContinue
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
