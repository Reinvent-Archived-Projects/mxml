// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once
#include "Node.h"
#include "Note.h"
#include "Position.h"
#include "Types.h"


namespace mxml {
namespace dom {

/**
 A tuplet element is present when a tuplet is to be displayed graphically, in addition to the sound data provided by
 the time-modification elements. The number attribute is used to distinguish nested tuplets. The bracket attribute is
 used to indicate the presence of a bracket. If unspecified, the results are implementation-dependent. The line-shape
 attribute is used to specify whether the bracket is straight or in the older curved or slurred style. It is straight
 by default.

 Whereas a time-modification element shows how the cumulative, sounding effect of tuplets and double-note tremolos
 compare to the written note type, the tuplet element describes how this is displayed. The tuplet element also provides
 more detailed representation information than the time-modification element, and is needed to represent nested tuplets
 and other complex tuplets accurately.

 The show-number attribute is used to display either the number of actual notes, the number of both actual and normal
 notes, or neither. It is actual by default. The show-type attribute is used to display either the actual type, both
 the actual and normal types, or neither. It is none by default.
 */
class Tuplet : public Node {
public:
    enum class Type {
        Start,
        Stop
    };

    enum class Show {
        Actual,
        Both,
        None
    };

    struct Portion {
        Optional<int> number;
        Optional<Note::Type> type;
    };

public:
    Tuplet() : type(), number(1), bracket(true), showNumber(Show::Actual), showType(Show::None) {}

    /**
     Determines how the actual part of the tuplet is displayed. If it is absent, its value is based on the
     time-modification element.
     */
    Portion actual;

    /**
     Determines how the normal part of the tuplet is displayed. If it is absent, its value is based on the
     time-modification element.
     */
    Portion normal;

    Type type;

    int number;
    Optional<bool> bracket;

    Show showNumber;
    Show showType;

    Position position;
    Optional<Placement> placement;
};

} // namespace dom
} // namespace mxml
