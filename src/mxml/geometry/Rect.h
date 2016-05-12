// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once
#include "Point.h"
#include "Size.h"

#include <algorithm>
#include <cmath>

namespace mxml {

class Rect {
public:
    Rect() : origin(), size() {}
    Rect(Point origin, Size size) : origin(origin), size(size) {}
    Rect(Point p1, Point p2) : origin(), size() {
        size.width = std::abs(p2.x - p1.x);
        size.height = std::abs(p2.y - p1.y);
        origin.x = std::min(p1.x, p2.x);
        origin.y = std::min(p1.y, p2.y);
    }
    
    const Point center() const {
        return Point(origin.x + size.width/2, origin.y + size.height/2);
    }
    void setCenter(const Point& p) {
        origin.x = p.x - size.width/2;
        origin.y = p.y - size.height/2;
    }
    
    const Point min() const {
        return origin;
    };
    const Point max() const {
        return Point(origin.x + size.width, origin.y + size.height);
    }
    
    bool operator==(const Rect& r) const {
        return origin == r.origin && size == r.size;
    }
    bool operator!=(const Rect& r) const {
        return origin != r.origin || size != r.size;
    }

    bool contains(const Point& p) const {
        auto mn = min();
        auto mx = max();
        return p.x >= mn.x && p.x <= mx.x && p.y >= mn.y && p.y <= mx.y;
    }

    Rect rotate(coord_t angle) const {
        Point corners[] = {
            {origin.x, origin.y},
            {origin.x + size.width, origin.y},
            {origin.x + size.width, origin.y + size.height},
            {origin.x, origin.y + size.height}
        };
        
        for (Point& corner : corners)
            corner = corner.rotate(angle);
        
        Point pmin, pmax;
        for (Point& corner : corners) {
            pmin.x = std::min(pmin.x, corner.x);
            pmin.y = std::min(pmin.y, corner.y);
            pmax.x = std::max(pmax.x, corner.x);
            pmax.y = std::max(pmax.y, corner.y);
        }
        
        return Rect(pmin, pmax);
    }
    
public:
    Point origin;
    Size size;
};

/** Determine if the reciever intersects another rectangle.
 */
inline bool intersect(const Rect& r1, const Rect& r2, float deltax = 0, float deltay = 0) {
    coord_t minx1 = r1.center().x - r1.size.width/2 + deltax/2;
    coord_t minx2 = r2.center().x - r2.size.width/2 + deltax/2;
    coord_t maxx1 = r1.center().x + r1.size.width/2 - deltax/2;
    coord_t maxx2 = r2.center().x + r2.size.width/2 - deltax/2;
    if (minx1 > maxx2 || maxx1 < minx2)
        return false;
    
    coord_t miny1 = r1.center().y - r1.size.height/2 + deltay/2;
    coord_t miny2 = r2.center().y - r2.size.height/2 + deltay/2;
    coord_t maxy1 = r1.center().y + r1.size.height/2 - deltay/2;
    coord_t maxy2 = r2.center().y + r2.size.height/2 - deltay/2;
    if (miny1 > maxy2 || maxy1 < miny2)
        return false;
    
    return true;
}

/** Return the smallest rectangle that contains both the receiver and another rectangle.
 */
inline Rect join(const Rect& r1, const Rect& r2) {
    Point min1 = r1.min();
    Point max1 = r1.max();
    Point min2 = r2.min();
    Point max2 = r2.max();
    coord_t minx = std::min(min1.x, min2.x);
    coord_t maxx = std::max(max1.x, max2.x);
    coord_t miny = std::min(min1.y, min2.y);
    coord_t maxy = std::max(max1.y, max2.y);
    return Rect(Point(minx, miny), Point(maxx, maxy));
}

} // namespace mxml
