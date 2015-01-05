//  Created by Alejandro Isaza on 2014-03-25.
//  Copyright (c) 2014 Venture Media Labs. All rights reserved.

#pragma once
#include "Node.h"
#include "Optional.h"
#include "StaffLayout.h"

namespace mxml {
namespace dom {

class Print : public Node {
public:
    Print() {}
    
    const Optional<float>& staffSpacing() const {
        return _staffSpacing;
    }
    void setStaffSpacing(const Optional<float>& staffSpacing) {
        _staffSpacing = staffSpacing;
    }
    
    const std::unique_ptr<StaffLayout>& staffLayout() const {
        return _staffLayout;
    }
    void setStaffLayout(std::unique_ptr<StaffLayout> layout) {
        _staffLayout = std::move(layout);
    }
    
private:
    Optional<float> _staffSpacing;
    std::unique_ptr<StaffLayout> _staffLayout;
};

} // namespace dom
} // namespace mxml
