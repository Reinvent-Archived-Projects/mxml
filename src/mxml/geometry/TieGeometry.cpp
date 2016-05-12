// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "TieGeometry.h"
#include <cmath>

namespace mxml {

using namespace dom;

const coord_t TieGeometry::kMaxHeight = 20;
const coord_t TieGeometry::kEndPointLineWidth = 0.5;

TieGeometry::TieGeometry()
: PlacementGeometry(absentOptional(Placement::Above), 1),
  _startLocation(),
  _stopLocation()
{
}

TieGeometry::TieGeometry(const Point& start, const Point& stop, Placement placement)
: PlacementGeometry(presentOptional(placement), 1),
  _startLocation(start),
  _stopLocation(stop)
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
    if (placement().value() == Placement::Above) {
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
