//  Created by Alejandro Isaza on 2014-05-05.
//  Copyright (c) 2014 Venture Media Labs. All rights reserved.

#include "TieGeometry.h"
#include <cmath>

namespace mxml {

using namespace dom;

const coord_t TieGeometry::kMaxHeight = 20;
const coord_t TieGeometry::kEndPointLineWidth = 0.5;

TieGeometry::TieGeometry()
: _startLocation(), _stopLocation(), _placement()
{
}

TieGeometry::TieGeometry(const Point& start, const Point& stop, Placement placement)
: _startLocation(start), _stopLocation(stop), _placement(placement)
{
    build();
}

void TieGeometry::build() {
    const coord_t dx = _stopLocation.x - _startLocation.x;
    const coord_t dy = _stopLocation.y - _startLocation.y;
    const coord_t angle = std::atan2(dy, dx);
    
    const coord_t curveLength = std::sqrt(dx*dx + dy*dy) + kEndPointLineWidth;
    const coord_t curveHeight = std::min(kMaxHeight, std::sqrt(curveLength));
    
    Rect bounds = Rect(Point(0, 0), Size(curveLength, curveHeight)).rotate(angle);
    setSize(bounds.size);
    
    coord_t hConstant = 0;
    coord_t vConstant = 0;
    coord_t vMultiplier = 0;
    if (_placement.value() == PLACEMENT_ABOVE) {
        if (angle > 0) {
            vMultiplier = 0;
            vConstant = curveHeight;
        } else {
            hConstant = curveHeight * std::sin(-angle);
            vMultiplier = 1;
        }
    } else {
        if (angle > 0) {
            hConstant = curveHeight * std::sin(angle);
            vMultiplier = 0;
        } else {
            vMultiplier = 1;
            vConstant = -curveHeight;
        }
    }
    setHorizontalAnchorPointValues(0, hConstant);
    setVerticalAnchorPointValues(vMultiplier, vConstant);
}

} // namespace mxml
