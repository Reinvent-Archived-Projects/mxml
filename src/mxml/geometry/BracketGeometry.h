//
//  BracketGeometry.hpp
//  pianomarvel
//
//  Created by nhatlee on 2/6/18.
//  Copyright © 2018 nhatlee. All rights reserved.
//

#pragma once
#include <mxml/dom/Bracket.h>
#include "SpanDirectionGeometry.h"

namespace mxml {
    
    class BracketGeometry : public SpanDirectionGeometry {
    public:
        static const Size kPedSize;
        static const Size kStarSize;
    public:
        BracketGeometry(const dom::Direction* start, const Point& startLocation, const dom::Direction* stop, const Point& stopLocation);
    };
    
} // namespace mxml
