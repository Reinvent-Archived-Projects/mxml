//  Created by Alejandro Isaza on 2014-04-21.
//  Copyright (c) 2014 Venture Media Labs. All rights reserved.

#pragma once
#include "Node.h"
#include "Types.h"

namespace mxml {
namespace dom {

class Tie : public Node {
public:
    Tie() : _type() {}
    
    StartStopContinue type() const {
        return _type;
    }
    void setType(StartStopContinue type) {
        _type = type;
    }
    
private:
    StartStopContinue _type;
};

} // namespace dom
} // namespace mxml
