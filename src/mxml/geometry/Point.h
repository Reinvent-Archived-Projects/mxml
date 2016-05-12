// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once
#include <cmath>
#include <initializer_list>
#include <stdexcept>

namespace mxml {

typedef float coord_t;

class Point {
public:
    typedef coord_t value_t;
    
public:
    Point() : x(), y() {}
    Point(value_t x, value_t y) : x(x), y(y) {}
    Point(std::initializer_list<value_t> l) {
        if (l.size() != 2)
            throw std::invalid_argument("initializer_list should contain 2 elements.");
        
        auto it = l.begin();
        x = *it;
        ++it;
        y = *it;
    }
    
    Point rotate(coord_t angle) const {
        const coord_t sin = std::sin(angle);
        const coord_t cos = std::cos(angle);
        return {x * cos - y * sin, x * sin + y * cos};
    }

public:
    value_t x;
    value_t y;
    
public:
    bool operator==(const Point& p) const {
        return x == p.x && y == p.y;
    }
    bool operator!=(const Point& p) const {
        return x != p.x || y != p.y;
    }
};

} // namespace mxml
