// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once
#include "Node.h"

namespace mxml {
namespace dom {

class Fermata : public Node {
public:
    enum class Type {
        Upright,
        Inverted
    };
    
    enum class Shape {
        Normal,
        Angled,
        Square
    };
    
public:
    Fermata() : _type(Type::Upright), _shape(Shape::Normal) {}
    Fermata(const Fermata& rhs) : _type(rhs.type()), _shape(rhs.shape()) {}
    
    Type type() const {
        return _type;
    }
    void setType(Type type) {
        _type = type;
    }
    
    Shape shape() const {
        return _shape;
    }
    void setShape(Shape shape) {
        _shape = shape;
    }
    
private:
    Type _type;
    Shape _shape;
};

} // namespace dom
} // namespace mxml
