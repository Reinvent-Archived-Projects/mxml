// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

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
    if (placement == dom::Placement::Below)
        setVerticalAnchorPointValues(0, 0);
    else
        setVerticalAnchorPointValues(1, 0);
}

} // namespace mxml
