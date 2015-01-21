//  Created by Alejandro Isaza on 2014-06-24.
//  Copyright (c) 2014 Venture Media Labs. All rights reserved.

#include "EndingGeometry.h"
#include "../dom/Measure.h"

namespace mxml {

const coord_t EndingGeometry::kMinWidth = 20;
const coord_t EndingGeometry::kHeight = 20;

EndingGeometry::EndingGeometry(const dom::Ending& startEnding, const Point& startLocation, const dom::Ending& stopEnding, const Point& stopLocation)
:   PlacementGeometry(presentOptional(dom::kPlacementAbove), 1),
    _startEnding(startEnding),
    _startLocation(startLocation),
    _stopEnding(stopEnding),
    _stopLocation(stopLocation) {
    setSize({std::max(kMinWidth, _stopLocation.x - _startLocation.x), kHeight});
    setHorizontalAnchorPointValues(0, 0);
    setVerticalAnchorPointValues(1, 0);
}

} // namespace
