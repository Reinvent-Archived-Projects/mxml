//  Created by Alejandro Isaza on 2014-03-25.
//  Copyright (c) 2014 Venture Media Labs. All rights reserved.

#pragma once
#include "Node.h"
#include "Optional.h"
#include "Scaling.h"
#include "StaffLayout.h"

namespace mxml {
namespace dom {

class Defaults : public Node {
public:
    Defaults() : _scaling(), _staffLayout() {}
    
    const Optional<Scaling>& scaling() const {
        return _scaling;
    }
    void setScaling(const Optional<Scaling>& scaling) {
        _scaling = scaling;
    }
    
    const Optional<StaffLayout>& staffLayout() const {
        return _staffLayout;
    }
    void setStaffLayout(const Optional<StaffLayout>& layout) {
        _staffLayout = layout;
    }
    
private:
    Optional<Scaling> _scaling;
    Optional<StaffLayout> _staffLayout;
};

} // namespace dom
} // namespace mxml
