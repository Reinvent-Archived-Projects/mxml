// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

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

    void setPlacement(dom::Optional<dom::Placement> placement) override {
        PlacementGeometry::setPlacement(placement);
        if (placement == dom::Placement::Below)
            setVerticalAnchorPointValues(0, 0);
        else
            setVerticalAnchorPointValues(1, 0);
    }
    
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

    bool bracket() const {
        return _bracket;
    }
    void setBracket(bool b) {
        _bracket = b;
    }

private:
    const dom::Tuplet& _startTuplet;
    const dom::Tuplet& _stopTuplet;
    Point _startLocation;
    Point _stopLocation;
    int _displayNumber;
    bool _bracket;
};

} // namespace mxml
