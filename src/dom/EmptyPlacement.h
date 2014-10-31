//  Created by Alejandro Isaza on 2014-04-23.
//  Copyright (c) 2014 Venture Media Labs. All rights reserved.

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
