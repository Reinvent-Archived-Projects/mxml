// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once
#include "VerticalResolver.h"
#include "HorizontalResolver.h"

namespace mxml {

    class CollisionHandler {

    public:
        CollisionHandler(const Geometry& geometry, const Metrics& metrics);

        /**
         Resolve all collisions in the set of geometries.
         */
        void resolveCollisions();

    private:
        VerticalResolver _verticalResolver;
        HorizontalResolver _horizontalResolver;
    };
}
