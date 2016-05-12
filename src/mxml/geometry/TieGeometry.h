// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once
#include "PlacementGeometry.h"
#include <mxml/dom/Types.h>
#include <mxml/dom/Optional.h>

namespace mxml {

class TieGeometry : public PlacementGeometry {
public:
    static const coord_t kMaxHeight;
    static const coord_t kEndPointLineWidth;
    
public:
    TieGeometry();
    TieGeometry(const Point& start, const Point& stop, dom::Placement placement);
    
    const Point& startLocation() const {
        return _startLocation;
    }
    void setStartLocation(Point p) {
        _startLocation = p;
        setLocation(_startLocation);
        build();
    }
    
    const Point& stopLocation() const {
        return _stopLocation;
    }
    void setStopLocation(Point p) {
        _stopLocation = p;
        build();
    }
    
private:
    void build();
    
private:
    Point _startLocation;
    Point _stopLocation;
};

} // namespace mxml
