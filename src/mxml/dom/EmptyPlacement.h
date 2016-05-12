// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once
#include "Node.h"
#include "Optional.h"
#include "Types.h"

namespace mxml {
namespace dom {

class EmptyPlacement : public Node {
public:
    EmptyPlacement() {}
    
    const Optional<Placement>& placement() const {
        return _placement;
    }
    void setPlacement(const Optional<Placement>& placement) {
        _placement = placement;
    }
    
private:
    Optional<Placement> _placement;
};

} // namespace dom
} // namespace mxml
