// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

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
    if (note.type() >= Note::Type::Whole)
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
