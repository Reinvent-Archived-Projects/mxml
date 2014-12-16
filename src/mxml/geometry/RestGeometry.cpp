//  Created by Alejandro Isaza on 2014-05-06.
//  Copyright (c) 2014 Venture Media Labs. All rights reserved.

#include "RestGeometry.h"

namespace mxml {

using namespace dom;

const Size RestGeometry::k128thSize  = {13, 62};
const Size RestGeometry::k64thSize   = {13, 62};
const Size RestGeometry::k32thSize   = {13, 44};
const Size RestGeometry::k16thSize   = {13, 44};
const Size RestGeometry::kEighthSize  = {12, 20};
const Size RestGeometry::kQuarterSize = {13, 20};
const Size RestGeometry::kHalfSize    = {13, 20};
const Size RestGeometry::kWholeSize   = {13, 20};
const Size RestGeometry::kBreveSize   = { 6, 20};
const Size RestGeometry::kLongSize    = { 6, 20};
const Size RestGeometry::kMaximaSize  = {17, 20};

RestGeometry::RestGeometry(const dom::Note& note) : _note(note) {
    setSize(Size(note));
    setStaff(note.staff());
}

Size RestGeometry::Size(const dom::Note& note) {
    if (!note.type().isPresent())
        return kHalfSize;
    
    switch (note.type().value()) {
        case Note::TYPE_1024TH:
        case Note::TYPE_512TH:
        case Note::TYPE_256TH:
        case Note::TYPE_128TH:
            return k128thSize;
            
        case Note::TYPE_64TH:
            return k64thSize;
            
        case Note::TYPE_32ND:
            return k32thSize;
            
        case Note::TYPE_16TH:
            return k16thSize;
            
        case Note::TYPE_EIGHTH:
            return kEighthSize;
            
        case Note::TYPE_QUARTER:
            return kQuarterSize;
            
        case Note::TYPE_HALF:
            return kHalfSize;
            
        case Note::TYPE_WHOLE:
            return kWholeSize;
            
        case Note::TYPE_BREVE:
            return kBreveSize;
            
        case Note::TYPE_LONG:
            return kLongSize;
            
        case Note::TYPE_MAXIMA:
            return kMaximaSize;
    }
}

} // namespace mxml
