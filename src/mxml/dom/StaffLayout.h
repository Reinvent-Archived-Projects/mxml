//  Created by Alejandro Isaza on 2014-03-25.
//  Copyright (c) 2014 Venture Media Labs. All rights reserved.

#pragma once
#include "Node.h"
#include "Optional.h"

namespace mxml {
namespace dom {

class StaffLayout : public Node {
public:
    StaffLayout() : _number(1), _staffDistance(65) {}
    
    int number() const {
        return _number;
    }
    void setNumber(int number) {
        _number = number;
    }
    
    const Optional<float>& staffDistance() const {
        return _staffDistance;
    }
    void setStaffDistance(const Optional<float>& distance) {
        _staffDistance = distance;
    }
    
private:
    int _number;
    Optional<float> _staffDistance;
};

} // namespace dom
} // namespace mxml
