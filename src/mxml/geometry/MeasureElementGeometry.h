//  Created by Alejandro Isaza on 2014-06-27.
//  Copyright (c) 2014 Venture Media Labs. All rights reserved.

#pragma once
#include "MeasureGeometry.h"

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

    MeasureGeometry* measureGeometry() const {
        return static_cast<MeasureGeometry*>(parentGeometry());
    }
    
private:
    int _staff;
};

} // namespace
