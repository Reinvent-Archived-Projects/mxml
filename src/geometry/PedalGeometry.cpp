//  Created by Alejandro Isaza on 2014-06-17.
//  Copyright (c) 2014 Venture Media Labs. All rights reserved.

#include "PedalGeometry.h"
#include "../dom/Pedal.h"

namespace mxml {
    const Size PedalGeometry::kPedSize = {30, 20};
    const Size PedalGeometry::kStarSize = {15, 15};

    PedalGeometry::PedalGeometry(const dom::Direction& start, const Point& startLocation, const dom::Direction& stop, const Point& stopLocation)
        : SpanDirectionGeometry(start, startLocation, stop, stopLocation)
    {
        Size size = Geometry::size();
        const dom::Pedal* pedal = dynamic_cast<const dom::Pedal*>(start.type());
        if (pedal->sign())
            size.width = std::max(size.width, kPedSize.width + kStarSize.width);
        else
            size.width = std::max(size.width, kPedSize.width + 2);
        size.height = kPedSize.height;
        setSize(size);
    }

} // namespace
