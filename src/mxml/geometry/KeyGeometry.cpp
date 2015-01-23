//  Created by Alejandro Isaza on 2014-05-13.
//  Copyright (c) 2014 Venture Media Labs. All rights reserved.

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
        size.width = key.fifths() * (AccidentalGeometry::kNaturalSize.width + kSpacing) - kSpacing;
    else if (key.fifths() < 0)
        size.width = -key.fifths() * (AccidentalGeometry::kFlatSize.width + kSpacing) - kSpacing;
    size.height = Metrics::staffHeight() + Metrics::kStaffLineSpacing + std::max(AccidentalGeometry::kFlatSize.height/2, AccidentalGeometry::kSharpSize.height/2);
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
    
    if (clef.sign() == dom::Clef::SIGN_F)
        y += 10;
    
    return y;
}

} // namespace mxml
