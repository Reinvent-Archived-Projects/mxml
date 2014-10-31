//  Created by Alejandro Isaza on 2014-05-05.
//  Copyright (c) 2014 Venture Media Labs. All rights reserved.

#include "DirectionGeometry.h"

namespace mxml {

DirectionGeometry::DirectionGeometry(const dom::Direction& direction) : _direction(direction) {
    dom::Dynamics* dynamics = dynamic_cast<dom::Dynamics*>(direction.type());
    if (dynamics) {
        setSize({static_cast<coord_t>(28 * dynamics->string().length()), 34});
    }
    
    dom::Words* words = dynamic_cast<dom::Words*>(direction.type());
    if (words) {
        setSize({static_cast<coord_t>(20 * words->contents().length()), 23});
    }

    setHorizontalAnchorPointValues(0, 0);
    if (direction.placement() == dom::PLACEMENT_ABOVE)
        setVerticalAnchorPointValues(1, 0);
    else
        setVerticalAnchorPointValues(0, 0);
}

} // namespace mxml
