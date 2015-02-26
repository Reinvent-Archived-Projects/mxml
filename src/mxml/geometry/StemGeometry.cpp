//  Created by Alejandro Isaza on 2014-05-05.
//  Copyright (c) 2014 Venture Media Labs. All rights reserved.

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
    assert(_stemDirection == dom::kStemUp || _stemDirection == dom::kStemDown);
    setSize(Size(note, _stemDirection, showFlags));
    setStaff(note.staff());
}

void StemGeometry::setStemDirection(dom::Stem direction) {
    assert(direction == dom::kStemUp || direction == dom::kStemDown);
    _stemDirection = direction;
    setSize(Size(_note, _stemDirection, _showFlags));
}

void StemGeometry::setShowFlags(bool show) {
    _showFlags = show;
    setSize(Size(_note, _stemDirection, _showFlags));
}

Size StemGeometry::Size(const Note& note, dom::Stem direction, bool flags) {
    coord_t width = NoteGeometry::kQuarterWidth;
    if (flags && note.type() <= Note::TYPE_EIGHTH && direction == dom::kStemUp)
        width += kFlagWidth - kNoFlagWidth;
    
    coord_t height = 0;
    if (note.type() >= Note::TYPE_WHOLE)
        height = 0;
    else if (note.type() >= Note::TYPE_16TH)
        height = kDefaultHeight;
    else if (note.type() == Note::TYPE_32ND)
        height = 40;
    else if (note.type() == Note::TYPE_64TH)
        height = 47;
    else if (note.type() <= Note::TYPE_128TH)
        height = 57;
    
    return {width, height + kVerticalOffset + NoteGeometry::kHeight/2};
}

} // namespace mxml
