// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

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
