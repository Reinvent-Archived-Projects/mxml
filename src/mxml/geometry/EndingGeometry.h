// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once
#include "PlacementGeometry.h"
#include <mxml/dom/Ending.h>

namespace mxml {

class EndingGeometry : public PlacementGeometry {
public:
    static const coord_t kMinWidth;
    static const coord_t kHeight;

public:
    EndingGeometry(const dom::Ending& startEnding, const Point& startLocation, const dom::Ending& stopEnding, const Point& stopLocation);

    const dom::Ending& startEnding() const {
        return _startEnding;
    }
    const Point& startLocation() const {
        return _startLocation;
    }

    const dom::Ending& stopEnding() const {
        return _stopEnding;
    }
    const Point& stopLocation() const {
        return _stopLocation;
    }

private:
    const dom::Ending& _startEnding;
    Point _startLocation;

    const dom::Ending& _stopEnding;
    Point _stopLocation;
};

} // namespace
