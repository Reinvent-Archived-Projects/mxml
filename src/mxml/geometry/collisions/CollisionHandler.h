//  Created by Alejandro Isaza on 2014-12-22.
//  Copyright (c) 2014 Venture Media Labs. All rights reserved.

#pragma once
#include "VerticalResolver.h"
#include "HorizontalResolver.h"

namespace mxml {

    class CollisionHandler {

    public:
        CollisionHandler(const PartGeometry& partGeometry);

        /**
         Resolve all collisions in the set of geometries.
         */
        void resolveCollisions();

    private:
        VerticalResolver _verticalResolver;
        HorizontalResolver _horizontalResolver;
    };
}
