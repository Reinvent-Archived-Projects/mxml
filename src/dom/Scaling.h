//  Created by Alejandro Isaza on 2014-03-25.
//  Copyright (c) 2014 Venture Media Labs. All rights reserved.

#pragma once
#include "Node.h"

namespace mxml {
namespace dom {

class Scaling : public Node {
public:
    Scaling() : _millimeters(7), _tenths(40) {}
    
    float millimeters() const {
        return _millimeters;
    }
    void setMillimeters(float millimeters) {
        _millimeters = millimeters;
    }
    
    float tenths() const {
        return _tenths;
    }
    void setTenths(float tenths) {
        _tenths = tenths;
    }
    
private:
    float _millimeters;
    float _tenths;
};

} // namespace dom
} // namespace mxml
