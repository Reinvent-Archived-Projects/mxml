// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "StemGeometry.h"
#include "NoteGeometry.h"
#include <cassert>

namespace mxml {

using dom::Note;

const coord_t StemGeometry::kNoFlagWidth = 1;
const coord_t StemGeometry::kFlagWidth = 11;
const coord_t StemGeometry::kMinHeight = 15;
const coord_t StemGeometry::kDefaultHeight = 34;
const coord_t StemGeometry::kVerticalOffset = 1;

StemGeometry::StemGeometry(const Note& note, bool showFlags)
: _note(note), _stemDirection(note.stem()), _showFlags(showFlags) {
    assert(_stemDirection == dom::Stem::Up || _stemDirection == dom::Stem::Down);
    setSize(Size(note, _stemDirection, showFlags));
    setStaff(note.staff());
}

void StemGeometry::setStemDirection(dom::Stem direction) {
    assert(direction == dom::Stem::Up || direction == dom::Stem::Down);
    _stemDirection = direction;
    setSize(Size(_note, _stemDirection, _showFlags));
}

void StemGeometry::setShowFlags(bool show) {
    _showFlags = show;
    setSize(Size(_note, _stemDirection, _showFlags));
}

Size StemGeometry::Size(const Note& note, dom::Stem direction, bool flags) {
    coord_t width = NoteGeometry::kQuarterWidth;
    if (flags && note.type() <= Note::Type::Eighth && direction == dom::Stem::Up)
        width += kFlagWidth - kNoFlagWidth;
    
    coord_t height = 0;
    if (note.type() >= Note::Type::Whole)
        height = 0;
    else if (note.type() >= Note::Type::_16th)
        height = kDefaultHeight;
    else if (note.type() == Note::Type::_32nd)
        height = 40;
    else if (note.type() == Note::Type::_64th)
        height = 47;
    else if (note.type() <= Note::Type::_128th)
        height = 57;
    
    return {width, height + kVerticalOffset + NoteGeometry::kHeight/2};
}

} // namespace mxml
