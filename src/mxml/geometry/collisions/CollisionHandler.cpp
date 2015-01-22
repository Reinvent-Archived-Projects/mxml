//  Created by Alejandro Isaza on 2014-12-22.
//  Copyright (c) 2014 Venture Media Labs. All rights reserved.

#include "CollisionHandler.h"

namespace mxml {

    CollisionHandler::CollisionHandler(const PartGeometry& partGeometry) : _verticalResolver(partGeometry), _horizontalResolver(partGeometry) {
    }
    
    void CollisionHandler::resolveCollisions() {
        _verticalResolver.resolveCollisions();
        _horizontalResolver.resolveCollisions();
    }
}
