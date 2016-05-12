// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Note.h"
#include "Attributes.h"
#include "Pitch.h"

#include <iostream>

namespace mxml {
namespace dom {

unsigned int Note::midiNumber() const {
    unsigned int number = 0;
    if (!pitch)
        return number;

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
