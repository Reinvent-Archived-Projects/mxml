// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "PedalGeometry.h"
#include "../dom/Pedal.h"

namespace mxml {
    const Size PedalGeometry::kPedSize = {30, 20};
    const Size PedalGeometry::kStarSize = {15, 15};

    PedalGeometry::PedalGeometry(const dom::Direction* start, const Point& startLocation, const dom::Direction* stop, const Point& stopLocation)
        : SpanDirectionGeometry(start, startLocation, stop, stopLocation)
    {
        Size size = Geometry::size();

        const dom::DirectionType* type = nullptr;
        if (start)
            type = start->type();
        else if (stop)
            type = stop->type();

        const dom::Pedal* pedal = dynamic_cast<const dom::Pedal*>(type);
        if (pedal->sign())
            size.width = std::max(size.width, kPedSize.width + kStarSize.width);
        else
            size.width = std::max(size.width, kPedSize.width + 2);
        size.height = kPedSize.height;
        setSize(size);
    }

} // namespace
