//  Created by Alejandro Isaza on 2014-12-18.
//  Copyright (c) 2014 Venture Media Labs. All rights reserved.

#pragma once
#include "Geometry.h"

#include <mxml/dom/Optional.h>
#include <mxml/dom/Types.h>

namespace mxml {

class PlacementGeometry : public Geometry {
public:
    explicit PlacementGeometry(dom::Optional<dom::Placement> placement, int staff) : _placement(placement), _staff(staff) {}

    const dom::Optional<dom::Placement> placement() const {
        return _placement;
    }
    void setPlacement(dom::Optional<dom::Placement> placement) {
        _placement = placement;
    }

    int staff() const {
        return _staff;
    }
    void setStaff(int staff) {
        _staff = staff;
    }

private:
    dom::Optional<dom::Placement> _placement;
    int _staff;
};
    
} // namespace mxml
