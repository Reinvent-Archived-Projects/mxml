// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once
#include "DirectionType.h"

namespace mxml {
namespace dom {

/**
 The octave shift type indicates where notes are shifted up or down from their true pitched values because of
 printing difficulty. Thus a treble clef line noted with 8va will be indicated with an octave-shift down from the
 pitch data indicated in the notes. A size of 8 indicates one octave; a size of 15 indicates two octaves.
 */
struct OctaveShift : public DirectionType {
public:
    enum class Type {
        Up,
        Down,
        Stop,
        Continue
    };

public:
    OctaveShift() : type(Type::Up), number(1), size(8)  {}

    bool span() const {
        return true;
    }

public:
    Type type;
    int number;
    int size;
};

}
}
