// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

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
