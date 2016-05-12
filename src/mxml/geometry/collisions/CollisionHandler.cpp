// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "CollisionHandler.h"

namespace mxml {

CollisionHandler::CollisionHandler(const Geometry& geometry, const Metrics& metrics)
: _verticalResolver(geometry, metrics),
  _horizontalResolver(geometry, metrics)
{}

void CollisionHandler::resolveCollisions() {
    _verticalResolver.resolveCollisions();
    _horizontalResolver.resolveCollisions();
}

} // namespace mxml
