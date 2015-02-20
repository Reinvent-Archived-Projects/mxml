//  Created by Alejandro Isaza on 2014-06-17.
//  Copyright (c) 2015 Venture Media Labs. All rights reserved.

#pragma once
#include "SpanDirectionGeometry.h"

namespace mxml {

    class OctaveShiftGeometry : public SpanDirectionGeometry {
    public:
        static const Size k8vaSize;

    public:
        OctaveShiftGeometry(const dom::Direction* start, const Point& startLocation, const dom::Direction* stop, const Point& stopLocation);
    };
    
} // namespace
