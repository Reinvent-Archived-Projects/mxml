//  Created by Alejandro Isaza on 2014-05-12.
//  Copyright (c) 2014 Venture Media Labs. All rights reserved.

#pragma once
#include "PlacementGeometry.h"
#include <mxml/dom/Direction.h>

namespace mxml {

class SpanDirectionGeometry : public PlacementGeometry {
public:
    static const coord_t kLineWidth;
    
public:
    SpanDirectionGeometry(const dom::Direction& start, const Point& startLocation, const dom::Direction& stop, const Point& stopLocation);
    
    const dom::Direction& startDirection() const {
        return _startDirection;
    }
    const Point& startLocation() const {
        return _startLocation;
    }
    
    const dom::Direction& stopDirection() const {
        return _stopDirection;
    }
    const Point& stopLocation() const {
        return _stopLocation;
    }
    
private:
    const dom::Direction& _startDirection;
    Point _startLocation;
    
    const dom::Direction& _stopDirection;
    Point _stopLocation;
};

} // namespace mxml
