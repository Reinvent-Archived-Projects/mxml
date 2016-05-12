// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "AccidentalGeometry.h"

namespace mxml {

const Size AccidentalGeometry::kNaturalSize = {6, 26};
const Size AccidentalGeometry::kSharpSize = {8, 26};
const Size AccidentalGeometry::kFlatSize = {8, 39};
const Size AccidentalGeometry::kDoubleSharpSize = {9, 9};
const Size AccidentalGeometry::kDoubleFlatSize = {16, 39};

AccidentalGeometry::AccidentalGeometry(int alter) : _alter(alter) {
    setSize(Size(alter));
}

Size AccidentalGeometry::Size(int alter) {
    switch (alter) {
        case 1:
            return kSharpSize;
            
        case -1:
            return kFlatSize;
            
        case 0:
            return kNaturalSize;
            
        case 2:
            return kDoubleSharpSize;
            
        case -2:
            return kDoubleFlatSize;

        default:
            return mxml::Size{};
    }
}

} // namespace mxml
