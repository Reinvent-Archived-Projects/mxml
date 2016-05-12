// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once
#include "PlacementGeometry.h"
#include <mxml/dom/Direction.h>

namespace mxml {

class SpanDirectionGeometry : public PlacementGeometry {
public:
    static const coord_t kLineWidth;
    
public:
    SpanDirectionGeometry(const dom::Direction* start, const Point& startLocation, const dom::Direction* stop, const Point& stopLocation);
    
    const dom::Direction* startDirection() const {
        return _startDirection;
    }
    const Point& startLocation() const {
        return _startLocation;
    }
    
    const dom::Direction* stopDirection() const {
        return _stopDirection;
    }
    const Point& stopLocation() const {
        return _stopLocation;
    }

    void setContinuation(bool b = true) {
        _continuation = b;
    }

    /**
     Determines if this is a geometry for a direction that started in a previous system. This affects how certain
     geometries are drawn. For instance (8va) instead of 8va.
     */
    bool isContinuation() const {
        return _continuation;
    }

    const dom::DirectionType* type() const {
        return _startDirection ? _startDirection->type() : _stopDirection ? _stopDirection->type() : nullptr;
    }
    
private:
    const dom::Direction* _startDirection;
    Point _startLocation;
    
    const dom::Direction* _stopDirection;
    Point _stopLocation;

    bool _continuation;
};

} // namespace mxml
