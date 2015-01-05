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
    
    const std::unique_ptr<Scaling>& scaling() const {
        return _scaling;
    }
    void setScaling(std::unique_ptr<Scaling> scaling) {
        _scaling = std::move(scaling);
    }
    
    const std::unique_ptr<StaffLayout>& staffLayout() const {
        return _staffLayout;
    }
    void setStaffLayout(std::unique_ptr<StaffLayout> layout) {
        _staffLayout = std::move(layout);
    }
    
private:
    std::unique_ptr<Scaling> _scaling;
    std::unique_ptr<StaffLayout> _staffLayout;
};

} // namespace dom
} // namespace mxml
