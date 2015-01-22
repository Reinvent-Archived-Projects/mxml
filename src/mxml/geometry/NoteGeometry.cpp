//  Created by Alejandro Isaza on 2014-05-05.
//  Copyright (c) 2014 Venture Media Labs. All rights reserved.

#include "NoteGeometry.h"

namespace mxml {

using dom::Note;

const coord_t NoteGeometry::kHeight = 10;
const coord_t NoteGeometry::kQuarterWidth = 12;
const coord_t NoteGeometry::kWholeWidth = 16;

NoteGeometry::NoteGeometry(const Note& note) : _note(note), _tieGeometry(nullptr) {
    setSize(Size(note));
    setStaff(note.staff());
}

Size NoteGeometry::Size(const Note& note) {
    if (note.type() >= Note::TYPE_WHOLE)
        return {kWholeWidth, kHeight};
    else
        return {kQuarterWidth, kHeight};
}

const TieGeometry* NoteGeometry::tieGeometry() const {
    return _tieGeometry;
}

void NoteGeometry::setTieGeometry(const TieGeometry* tieGeometry) {
    _tieGeometry = tieGeometry;
}

} // namespace mxml
