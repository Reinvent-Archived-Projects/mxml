// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "DotGeometry.h"

namespace mxml {

const Size DotGeometry::kSize = {3, 3};

DotGeometry::DotGeometry() {
    setSize(kSize);
}

} // namespace mxml
