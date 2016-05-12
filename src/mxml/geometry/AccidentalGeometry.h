// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once
#include "Geometry.h"
#include <mxml/dom/Accidental.h>

namespace mxml {

class AccidentalGeometry : public Geometry {
public:
    static const Size kNaturalSize;
    static const Size kSharpSize;
    static const Size kFlatSize;
    static const Size kDoubleSharpSize;
    static const Size kDoubleFlatSize;
    
public:
    explicit AccidentalGeometry(int alter);
    
    int alter() const {
        return _alter;
    }
    
    static Size Size(int alter);
    
private:
    int _alter;
};

} // namespace mxml
