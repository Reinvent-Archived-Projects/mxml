//  Copyright (c) 2014 Venture Media Labs. All rights reserved.

#include "CodaGeometry.h"

namespace mxml {

const Size CodaGeometry::kCodaSize = {24, 28};

CodaGeometry::CodaGeometry(const dom::Coda& coda)
: PlacementGeometry(dom::Optional<dom::Placement>(dom::kPlacementAbove, true), 1)
{
    setSize(kCodaSize);
}

} // namespace mxml
