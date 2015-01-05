//  Created by Alejandro Isaza on 2014-04-17.
//  Copyright (c) 2014 Venture Media Labs. All rights reserved.

#pragma once
#include "Node.h"

namespace mxml {
namespace dom {

class Accidental : public Node {
public:
    enum Type {
        TYPE_SHARP,
        TYPE_FLAT,
        TYPE_NATURAL,
        TYPE_DOUBLE_SHARP,
        TYPE_DOUBLE_FLAT
    };
    
public:
    Accidental() : _type(TYPE_SHARP) {}
    Accidental(Type type) : _type(type) {}
    Accidental(const Accidental& rhs) : _type(rhs.type()) {}
    
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
