//  Created by Alejandro Isaza on 2014-03-04.
//  Copyright (c) 2014 Venture Media Labs Inc. All rights reserved.

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
