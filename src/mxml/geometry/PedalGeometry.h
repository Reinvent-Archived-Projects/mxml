// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

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
