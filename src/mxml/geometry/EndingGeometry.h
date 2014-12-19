//  Created by Alejandro Isaza on 2014-06-24.
//  Copyright (c) 2014 Venture Media Labs. All rights reserved.

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
