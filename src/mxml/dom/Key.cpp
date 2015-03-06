//  Created by Alejandro Isaza on 2015-01-06.
//  Copyright (c) 2015 Venture Media Labs. All rights reserved.

#include "Key.h"

#include <algorithm>

namespace mxml {
namespace dom {

int Key::alter(Pitch::Step step) const {
    // Only Major and Minor modes are supported
    if (_mode != Mode::Major && _mode != Mode::Minor)
        return 0;

    Pitch::Step order[] = {
        Pitch::Step::F,
        Pitch::Step::C,
        Pitch::Step::G,
        Pitch::Step::D,
        Pitch::Step::A,
        Pitch::Step::E,
        Pitch::Step::B
    };

    static const auto stepCount = sizeof(order) / sizeof(Pitch::Step);
    auto stepIter = std::find(order, order + stepCount, step);
    if (stepIter == order + stepCount)
        return 0;

    auto index = stepIter - order;
    if (_fifths < 0 && stepCount - index <= -_fifths)
        return -1;
    else if (_fifths > 0 && index < _fifths)
        return 1;

    return 0;
}

}
}
