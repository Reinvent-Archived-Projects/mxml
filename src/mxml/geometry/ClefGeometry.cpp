// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "ClefGeometry.h"

#include <cassert>


namespace mxml {

const Size ClefGeometry::kSize = {28, 80};

ClefGeometry::ClefGeometry(const dom::Clef& clef) : _clef(clef) {
    assert(&clef);
    setSize(kSize);
    setStaff(_clef.number());
}

} // namespace mxml
