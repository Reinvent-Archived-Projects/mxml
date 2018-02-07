// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "BracketGeometry.h"
#include <cassert>


namespace mxml {
    
    const Size BracketGeometry::kPedSize = {30, 20};
    const Size BracketGeometry::kStarSize = {15, 15};
    
    BracketGeometry::BracketGeometry(const dom::Direction* start, const Point& startLocation, const dom::Direction* stop, const Point& stopLocation)
    : SpanDirectionGeometry(start, startLocation, stop, stopLocation)
    {
        Size size = Geometry::size();
        
        const dom::DirectionType* type = nullptr;
        if (start)
            type = start->type();
        else if (stop)
            type = stop->type();
        
        const dom::Bracket* bracket = dynamic_cast<const dom::Bracket*>(type);
        if (bracket->sign())
            size.width = std::max(size.width, kPedSize.width + kStarSize.width);
        else
            size.width = std::max(size.width, kPedSize.width + 2);
        size.height = kPedSize.height;
        setSize(size);
    }

    
} // namespace mxml
