// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once
#include "Geometry.h"


namespace mxml {

class MeasureElementGeometry : public Geometry {
public:
    MeasureElementGeometry() : _staff(0) {}
    
    int staff() const {
        return _staff;
    }
    void setStaff(int staff) {
        _staff = staff;
    }

private:
    int _staff;
};

} // namespace
