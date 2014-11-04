//  Created by Alejandro Isaza on 2014-05-05.
//  Copyright (c) 2014 Venture Media Labs. All rights reserved.

#include "AccidentalGeometry.h"

namespace mxml {

using dom::Accidental;

const Size AccidentalGeometry::kNaturalSize = {6, 26};
const Size AccidentalGeometry::kSharpSize = {8, 26};
const Size AccidentalGeometry::kFlatSize = {8, 39};
const Size AccidentalGeometry::kDoubleSharpSize = {9, 9};
const Size AccidentalGeometry::kDoubleFlatSize = {16, 9};

AccidentalGeometry::AccidentalGeometry(const Accidental& accidental) : _accidental(accidental) {
    setSize(Size(accidental));
}

Size AccidentalGeometry::Size(const Accidental& accidental) {
    switch (accidental.type()) {
        case Accidental::TYPE_SHARP:
            return kSharpSize;
            
        case Accidental::TYPE_FLAT:
            return kFlatSize;
            
        case Accidental::TYPE_NATURAL:
            return kNaturalSize;
            
        case Accidental::TYPE_DOUBLE_SHARP:
            return kDoubleSharpSize;
            
        case Accidental::TYPE_DOUBLE_FLAT:
            return kDoubleFlatSize;
    }
}

} // namespace mxml
