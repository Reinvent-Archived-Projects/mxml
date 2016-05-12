// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

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
