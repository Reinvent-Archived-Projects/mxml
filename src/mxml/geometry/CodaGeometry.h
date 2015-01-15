//  Copyright (c) 2014 Venture Media Labs. All rights reserved.

#pragma once
#include "PlacementGeometry.h"
#include <mxml/dom/DirectionType.h>

namespace mxml {

class CodaGeometry : public PlacementGeometry {
public:
    static const Size kCodaSize;
    
public:
    explicit CodaGeometry(const dom::Coda& coda);
    
};

} // namespace mxml
