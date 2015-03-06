//  Created by Alejandro Isaza on 2014-04-23.
//  Copyright (c) 2014 Venture Media Labs. All rights reserved.

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
