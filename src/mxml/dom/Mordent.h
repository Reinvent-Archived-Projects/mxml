//  Created by Alejandro Isaza on 2014-06-23.
//  Copyright (c) 2014 Venture Media Labs. All rights reserved.

#pragma once
#include "EmptyPlacement.h"
#include "Types.h"

namespace mxml {
namespace dom {

class Mordent : public EmptyPlacement {
public:
    Mordent() : _long(false) {}

    bool isLong() const {
        return _long;
    }
    void setLong(bool l) {
        _long = l;
    }
    
private:
    bool _long;
};

} // namespace dom
} // namespace mxml