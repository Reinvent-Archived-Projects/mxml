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