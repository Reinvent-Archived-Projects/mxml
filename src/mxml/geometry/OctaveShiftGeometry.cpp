// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "OctaveShiftGeometry.h"
#include <mxml/dom/OctaveShift.h>

namespace mxml {

const Size OctaveShiftGeometry::k8vaSize = {30, 20};

OctaveShiftGeometry::OctaveShiftGeometry(const dom::Direction* start, const Point& startLocation, const dom::Direction* stop, const Point& stopLocation)
: SpanDirectionGeometry(start, startLocation, stop, stopLocation)
{
    Size size = Geometry::size();
    size.width = std::max(size.width, k8vaSize.width + 2);
    size.height = k8vaSize.height;
    setSize(size);
}

} // namespace
