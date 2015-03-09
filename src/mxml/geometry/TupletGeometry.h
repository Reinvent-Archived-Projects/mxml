//  Created by Alejandro Isaza on 2015-03-06.
//  Copyright (c) 2015 Venture Media Labs. All rights reserved.

#pragma once
#include "PlacementGeometry.h"
#include <mxml/dom/Tuplet.h>
#include <mxml/dom/Types.h>


namespace mxml {

class TupletGeometry : public PlacementGeometry {
public:
    constexpr static const coord_t kTupletHeight = 10;

public:
    TupletGeometry(const dom::Tuplet& startTuplet, const Point& start, const dom::Tuplet& stopTuplet, const Point& stop, dom::Placement placement, int staff);

    const dom::Tuplet& startTuplet() const {
        return _startTuplet;
    }
    const dom::Tuplet& stopTuplet() const {
        return _stopTuplet;
    }

    const Point& startLocation() const {
        return _startLocation;
    }
    void setStartLocation(Point p) {
        _startLocation = p;
        setLocation(_startLocation);
    }

    const Point& stopLocation() const {
        return _stopLocation;
    }
    void setStopLocation(Point p) {
        _stopLocation = p;
    }

    int displayNumber() const {
        return _displayNumber;
    }
    void setDisplayNumber(int number) {
        _displayNumber = number;
    }
    
private:
    const dom::Tuplet& _startTuplet;
    const dom::Tuplet& _stopTuplet;
    Point _startLocation;
    Point _stopLocation;
    int _displayNumber;
};

} // namespace mxml
