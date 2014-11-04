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
    if (!_pitch.isPresent())
        return number;

    const Pitch& pitch = _pitch;
    number = (1 + pitch.octave()) * 12;

    switch (pitch.step()) {
        case Pitch::STEP_C:
            break;

        case Pitch::STEP_D:
            number += 2;
            break;

        case Pitch::STEP_E:
            number += 4;
            break;

        case Pitch::STEP_F:
            number += 5;
            break;

        case Pitch::STEP_G:
            number += 7;
            break;

        case Pitch::STEP_A:
            number += 9;
            break;

        case Pitch::STEP_B:
            number += 11;
            break;
    }

    return number + pitch.alter();
}

} // namespace dom
} // namespace mxml
