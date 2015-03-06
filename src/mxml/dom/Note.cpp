//  Created by Alejandro Isaza on 2014-08-05.
//  Copyright (c) 2014 Venture Media Labs. All rights reserved.

#include "Note.h"
#include "Attributes.h"
#include "Pitch.h"

#include <iostream>

namespace mxml {
namespace dom {

unsigned int Note::midiNumber() const {
    unsigned int number = 0;
    if (!pitch())
        return number;

    const auto& pitch = _pitch;
    number = (1 + pitch->octave()) * 12;

    switch (pitch->step()) {
        case Pitch::Step::C:
            break;

        case Pitch::Step::D:
            number += 2;
            break;

        case Pitch::Step::E:
            number += 4;
            break;

        case Pitch::Step::F:
            number += 5;
            break;

        case Pitch::Step::G:
            number += 7;
            break;

        case Pitch::Step::A:
            number += 9;
            break;

        case Pitch::Step::B:
            number += 11;
            break;
    }

    return number + pitch->alter();
}

} // namespace dom
} // namespace mxml
