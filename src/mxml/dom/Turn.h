//  Created by Alejandro Isaza on 2014-06-23.
//  Copyright (c) 2014 Venture Media Labs. All rights reserved.

#pragma once
#include "EmptyPlacement.h"
#include "Types.h"

namespace mxml {
namespace dom {

class Turn : public EmptyPlacement {
public:
    Turn() : _slash(false) {}
    
    bool slash() const {
        return _slash;
    }
    void setSlash(bool slash) {
        _slash = slash;
    }
    
private:
    bool _slash;
};

} // namespace dom
} // namespace mxml
