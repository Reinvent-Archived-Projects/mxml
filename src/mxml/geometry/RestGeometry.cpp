// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "RestGeometry.h"

namespace mxml {

using namespace dom;

const Size RestGeometry::k128thSize  = {13, 62};
const Size RestGeometry::k64thSize   = {13, 62};
const Size RestGeometry::k32thSize   = {13, 44};
const Size RestGeometry::k16thSize   = {13, 44};
const Size RestGeometry::kEighthSize  = {12, 20};
const Size RestGeometry::kQuarterSize = {13, 30};
const Size RestGeometry::kHalfSize    = {13, 10};
const Size RestGeometry::kWholeSize   = {13, 10};
const Size RestGeometry::kBreveSize   = { 6, 10};
const Size RestGeometry::kLongSize    = { 6, 20};
const Size RestGeometry::kMaximaSize  = {17, 20};

RestGeometry::RestGeometry(const dom::Note& note) : _note(note) {
    setSize(Size(note));
    setStaff(note.staff());
}

Size RestGeometry::Size(const dom::Note& note) {
    if (!note.type().isPresent())
        return kWholeSize;
    
    switch (note.type().value()) {
        case Note::Type::_1024th:
        case Note::Type::_512th:
        case Note::Type::_256th:
        case Note::Type::_128th:
            return k128thSize;
            
        case Note::Type::_64th:
            return k64thSize;
            
        case Note::Type::_32nd:
            return k32thSize;
            
        case Note::Type::_16th:
            return k16thSize;
            
        case Note::Type::Eighth:
            return kEighthSize;
            
        case Note::Type::Quarter:
            return kQuarterSize;
            
        case Note::Type::Half:
            return kHalfSize;
            
        case Note::Type::Whole:
            return kWholeSize;
            
        case Note::Type::Breve:
            return kBreveSize;
            
        case Note::Type::Long:
            return kLongSize;
            
        case Note::Type::Maxima:
            return kMaximaSize;
    }
}

} // namespace mxml
