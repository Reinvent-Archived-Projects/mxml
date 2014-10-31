//  Created by Alejandro Isaza on 2014-03-04.
//  Copyright (c) 2014 Venture Media Labs Inc. All rights reserved.

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
