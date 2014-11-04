//  Created by Alejandro Isaza on 2014-05-01.
//  Copyright (c) 2014 Venture Media Labs. All rights reserved.

#pragma once
#include "Node.h"

namespace mxml {
namespace dom {

class Repeat : public Node {
public:
    enum Direction {
        DIRECTION_BACKWARD,
        DIRECTION_FORWARD
    };
    
public:
    Repeat() : _direction(DIRECTION_BACKWARD), _times(0) {}
    
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
