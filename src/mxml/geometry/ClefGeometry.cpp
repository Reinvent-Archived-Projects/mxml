//  Created by Alejandro Isaza on 2014-04-29.
//  Copyright (c) 2014 Venture Media Labs. All rights reserved.

#include "ClefGeometry.h"

namespace mxml {

const Size ClefGeometry::kSize = {28, 80};

ClefGeometry::ClefGeometry(const dom::Clef& clef) : _clef(clef) {
    assert(&clef);
    setSize(kSize);
    setStaff(_clef.number());
}

} // namespace mxml
