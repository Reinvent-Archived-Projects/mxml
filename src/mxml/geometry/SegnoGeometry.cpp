//  Created by Alejandro Isaza on 2014-05-05.
//  Copyright (c) 2014 Venture Media Labs. All rights reserved.

#include "SegnoGeometry.h"

namespace mxml {

const Size SegnoGeometry::kSegnoSize = {19, 25};

SegnoGeometry::SegnoGeometry(const dom::Segno& segno)
: PlacementGeometry(dom::Optional<dom::Placement>(dom::PLACEMENT_ABOVE, true), 1)
{
    setSize(kSegnoSize);
}

} // namespace mxml
