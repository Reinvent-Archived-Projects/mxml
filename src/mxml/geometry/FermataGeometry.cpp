//  Created by Alejandro Isaza on 2014-06-16.
//  Copyright (c) 2014 Venture Media Labs. All rights reserved.

#include "FermataGeometry.h"

namespace mxml {

const Size FermataGeometry::kSize = {22, 12};

FermataGeometry::FermataGeometry(const dom::Fermata& fermata) : _fermata(fermata) {
    setSize(kSize);
}

} // namespace
