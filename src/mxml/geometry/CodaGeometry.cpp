// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "CodaGeometry.h"

namespace mxml {

const Size CodaGeometry::kCodaSize = {24, 28};

CodaGeometry::CodaGeometry(const dom::Coda& coda)
: PlacementGeometry(dom::Optional<dom::Placement>(dom::Placement::Above, true), 1)
{
    setSize(kCodaSize);
}

} // namespace mxml
