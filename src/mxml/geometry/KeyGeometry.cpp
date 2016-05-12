// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "KeyGeometry.h"
#include "AccidentalGeometry.h"

#include <mxml/Metrics.h>

namespace mxml {

const coord_t KeyGeometry::kSpacing = 2;

KeyGeometry::KeyGeometry(const dom::Key& key, const dom::Clef& clef) : _key(key), _clef(clef), _natural(false) {
    assert(&key);
    assert(&clef);
    setSize(keySize(key));
    setStaff(_key.number());
}

Size KeyGeometry::keySize(const dom::Key& key) {
    Size size;
    if (key.fifths() > 0)
        size.width = key.fifths() * (AccidentalGeometry::kSharpSize.width + kSpacing) - kSpacing;
    else if (key.fifths() == 0)
        size.width = 0;
    else if (key.fifths() < 0)
        size.width = -key.fifths() * (AccidentalGeometry::kFlatSize.width + kSpacing) - kSpacing;
    size.height = Metrics::staffHeight() + Metrics::kStaffLineSpacing + std::max(AccidentalGeometry::kFlatSize.height, AccidentalGeometry::kSharpSize.height);
    return size;
}

coord_t KeyGeometry::keyStaffY(int fifth, const dom::Clef& clef) {
    static const coord_t kSharpOffsets[] = {0, 15, -5, 10, 25, 5, 20};
    static const coord_t kFlatOffsets[] = {20, 5, 25, 10, 30, 15, 35};
    static const int kCount = sizeof(kFlatOffsets) / sizeof(coord_t);
    
    coord_t y = 0;
    if (fifth < 0)
        y = kFlatOffsets[(-fifth - 1) % kCount];
    else if (fifth > 0)
        y = kSharpOffsets[(fifth - 1) % kCount];
    
    if (clef.sign() == dom::Clef::Sign::F)
        y += 10;
    
    return y;
}

} // namespace mxml
