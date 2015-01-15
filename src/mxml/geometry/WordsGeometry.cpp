//  Created by Alejandro Isaza on 2014-05-05.
//  Copyright (c) 2014 Venture Media Labs. All rights reserved.

#include "WordsGeometry.h"

namespace mxml {

WordsGeometry::WordsGeometry(const dom::Direction& direction)
: PlacementGeometry(direction.placement(), direction.staff()), _direction(direction), _dynamics(false)
{
    dom::Dynamics* dynamics = dynamic_cast<dom::Dynamics*>(direction.type());
    if (dynamics) {
        setSize({static_cast<coord_t>(28 * dynamics->string().length()), 34});
        _dynamics = true;
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
    
const std::string* WordsGeometry::contents() const {
    mxml::dom::Dynamics* dynamics = dynamic_cast<mxml::dom::Dynamics*>(_direction.type());
    if (dynamics)
        return &dynamics->string();
    
    mxml::dom::Words* words = dynamic_cast<mxml::dom::Words*>(_direction.type());
    if (words)
        return &words->contents();
    
    return nullptr;
}

} // namespace mxml
