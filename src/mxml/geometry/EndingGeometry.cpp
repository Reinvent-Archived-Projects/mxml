// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "EndingGeometry.h"
#include "../dom/Measure.h"

namespace mxml {

const coord_t EndingGeometry::kMinWidth = 20;
const coord_t EndingGeometry::kHeight = 20;

EndingGeometry::EndingGeometry(const dom::Ending& startEnding, const Point& startLocation, const dom::Ending& stopEnding, const Point& stopLocation)
:   PlacementGeometry(presentOptional(dom::Placement::Above), 1),
    _startEnding(startEnding),
    _startLocation(startLocation),
    _stopEnding(stopEnding),
    _stopLocation(stopLocation) {
    setSize({std::max(kMinWidth, _stopLocation.x - _startLocation.x), kHeight});
    setHorizontalAnchorPointValues(0, 0);
    setVerticalAnchorPointValues(1, 0);
}

} // namespace
