//  Created by Alejandro Isaza on 2014-05-05.
//  Copyright (c) 2014 Venture Media Labs. All rights reserved.

#pragma once
#include "PlacementGeometry.h"
#include <mxml/dom/DirectionType.h>

namespace mxml {

class SegnoGeometry : public PlacementGeometry {
public:
    static const Size kSegnoSize;
    
public:
    explicit SegnoGeometry(const dom::Segno& segno);
    
};

} // namespace mxml
