//  Copyright (c) 2015 Venture Media Labs. All rights reserved.

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
    enum Type {
        kUp,
        kDown,
        kStop,
        kContinue
    };

public:
    OctaveShift() : type(kUp), number(1), size(8)  {}

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
