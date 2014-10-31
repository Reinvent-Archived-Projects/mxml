//  Created by Alejandro Isaza on 2014-05-05.
//  Copyright (c) 2014 Venture Media Labs. All rights reserved.

#include "TimeSignatureGeometry.h"

namespace mxml {

const coord_t TimeSignatureGeometry::kVerticalSpacing = 1;
const coord_t TimeSignatureGeometry::kHorizontalSpacing = 0;
const Size TimeSignatureGeometry::kDigitSize = {15, 19};
const Size TimeSignatureGeometry::kCommonSize = {16, 20};

TimeSignatureGeometry::TimeSignatureGeometry(const dom::Time& time) : _time(time) {
    setSize(Size(time));
    setStaff(_time.number());
}

Size TimeSignatureGeometry::Size(const dom::Time& time) {
    if (time.symbol() == dom::Time::SYMBOL_COMMON)
        return kCommonSize;
    
    class Size s;
    s.width = kDigitSize.width * std::max(numberOfDigits(time.beats()),
                                          numberOfDigits(time.beatType()));
    s.height = kDigitSize.height + kVerticalSpacing + kDigitSize.height;
    return s;
}

int TimeSignatureGeometry::numberOfDigits(int number) {
    int n = 1;
    while (number >= 10) {
        number /= 10;
        n += 1;
    }
    return n;
}

} // namespace mxml
