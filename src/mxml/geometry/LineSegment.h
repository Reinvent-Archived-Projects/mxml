// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once
#include "Point.h"

namespace mxml {

class LineSegment {
public:
    LineSegment() : _start(), _end() {}
    LineSegment(const Point& start, const Point& end) : _start(start), _end(end) {}
    LineSegment(std::initializer_list<Point> l) {
        if (l.size() != 2)
            throw std::invalid_argument("initializer_list should contain 2 elements.");
        
        auto it = l.begin();
        _start = *it;
        ++it;
        _end = *it;
    }
    
    const Point& start() const {
        return _start;
    }
    Point& start() {
        return _start;
    }
    const Point& end() const {
        return _end;
    }
    Point& end() {
        return _end;
    }
    
    /**
     Return the point with parameter t in the line segment, with 0 ≤ t ≤ 1.
     */
    Point point(double t) {
        return Point(
            _start.x + (_end.x - _start.x) * t,
            _start.y + (_end.y - _start.y) * t
        );
    }
    
    bool operator==(const LineSegment& ls) const {
        return _start == ls._start && _end == ls._end;
    }
    bool operator!=(const LineSegment& ls) const {
        return !operator==(ls);
    }
    
private:
    Point _start;
    Point _end;
};

} // namespace mxml
