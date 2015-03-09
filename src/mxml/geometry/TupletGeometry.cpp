//  Created by Alejandro Isaza on 2015-03-06.
//  Copyright (c) 2015 Venture Media Labs. All rights reserved.

#include "TupletGeometry.h"

namespace mxml {

TupletGeometry::TupletGeometry(const dom::Tuplet& startTuplet, const Point& start,
                               const dom::Tuplet& stopTuplet, const Point& stop,
                               dom::Placement placement, int staff)
: PlacementGeometry({placement, true}, staff),
  _startTuplet(startTuplet),
  _startLocation(start),
  _stopTuplet(stopTuplet),
  _stopLocation(stop),
  _displayNumber(0),
  _bracket(true)
{
    setSize({stop.x - start.x, kTupletHeight});
    setLocation(start);
    setHorizontalAnchorPointValues(0, 0);
    setVerticalAnchorPointValues(0, 0);
}

} // namespace mxml
