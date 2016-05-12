// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

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
