//  Created by Alejandro Isaza on 2014-06-17.
//  Copyright (c) 2014 Venture Media Labs. All rights reserved.

#pragma once
#include "SpanDirectionGeometry.h"

namespace mxml {

class PedalGeometry : public SpanDirectionGeometry {
public:
    static const Size kPedSize;
    static const Size kStarSize;

public:
    PedalGeometry(const dom::Direction* start, const Point& startLocation, const dom::Direction* stop, const Point& stopLocation);
};

} // namespace
