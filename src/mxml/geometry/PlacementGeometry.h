// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

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
    virtual void setPlacement(dom::Optional<dom::Placement> placement) {
        _placement = placement;
    }

    int staff() const {
        return _staff;
    }
    virtual void setStaff(int staff) {
        _staff = staff;
    }

private:
    dom::Optional<dom::Placement> _placement;
    int _staff;
};
    
} // namespace mxml
