// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

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
    if (time.symbol() == dom::Time::Symbol::Common)
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
