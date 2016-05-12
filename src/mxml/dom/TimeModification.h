// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once
#include "Node.h"


namespace mxml {
namespace dom {

/**
 Time modification indicates tuplets, double-note tremolos, and other durational changes. A time-modification element
 shows how the cumulative, sounding effect of tuplets and double-note tremolos compare to the written note type
 represented by the type and dot elements. Nested tuplets and other notations that use more detailed information need
 both the time-modification and tuplet elements to be represented accurately.
 */
class TimeModification : public Node {
public:
    /**
     The actual-notes element describes how many notes are played in the time usually occupied by the number in the
     normal-notes element.
     */
    int actualNotes;

    /**
     The normal-notes element describes how many notes are usually played in the time occupied by the number in the
     actual-notes element.
     */
    int normalNotes;
};

} // namespace dom
} // namespace mxml
