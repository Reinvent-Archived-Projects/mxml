// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "SegnoGeometry.h"

namespace mxml {

const Size SegnoGeometry::kSegnoSize = {19, 25};

SegnoGeometry::SegnoGeometry(const dom::Segno& segno)
: PlacementGeometry(dom::Optional<dom::Placement>(dom::Placement::Above, true), 1)
{
    setSize(kSegnoSize);
}

} // namespace mxml
