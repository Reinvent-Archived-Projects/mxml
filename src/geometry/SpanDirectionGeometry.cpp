//  Created by Alejandro Isaza on 2014-05-12.
//  Copyright (c) 2014 Venture Media Labs. All rights reserved.

#include "SpanDirectionGeometry.h"
#include "dom/Wedge.h"
#include "dom/Pedal.h"

#include <algorithm>
#include <cassert>

namespace mxml {

using namespace dom;

const coord_t SpanDirectionGeometry::kLineWidth = 1;

SpanDirectionGeometry::SpanDirectionGeometry(const Direction& start, const Point& startLocation, const Direction& stop, const Point& stopLocation)
: _startDirection(start),
  _startLocation(startLocation),
  _stopDirection(stop),
  _stopLocation(stopLocation)
{
    assert(typeid(start.type()) == typeid(stop.type()));
    setHorizontalAnchorPointValues(0, 0);
    
    Size size;
    size.width = stopLocation.x - startLocation.x;
    
    if (const Wedge* startWedge = dynamic_cast<const Wedge*>(start.type())) {
        const Wedge* stopWedge = dynamic_cast<const Wedge*>(stop.type());
        size.height = 2*kLineWidth + std::max(startWedge->spread(), stopWedge->spread());
    }
    setSize(size);
}

} // namespace mxml
