//  Created by Alejandro Isaza on 2014-05-01.
//  Copyright (c) 2014 Venture Media Labs. All rights reserved.

#pragma once
#include "Node.h"

namespace mxml {
namespace dom {

class Repeat : public Node {
public:
    enum class Direction {
        Backward,
        Forward
    };
    
public:
    Repeat() : _direction(Direction::Backward), _times(0) {}
    
    Direction direction() const {
        return _direction;
    }
    void setDirection(Direction dir) {
        _direction = dir;
    }
    
    int times() const {
        return _times;
    }
    void setTimes(int times) {
        _times = times;
    }
    
private:
    Direction _direction;
    int _times;
};

} // namespace dom
} // namespace mxml
