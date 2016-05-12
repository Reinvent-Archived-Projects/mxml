// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "FermataGeometry.h"

namespace mxml {

const Size FermataGeometry::kSize = {22, 12};

FermataGeometry::FermataGeometry(const dom::Fermata& fermata) : _fermata(fermata) {
    setSize(kSize);
}

} // namespace
