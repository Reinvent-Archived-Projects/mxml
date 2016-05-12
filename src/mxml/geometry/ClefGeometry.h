// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once
#include "MeasureElementGeometry.h"
#include <mxml/dom/Clef.h>

namespace mxml {

class ClefGeometry : public MeasureElementGeometry {
public:
    static const Size kSize;
    
public:
    explicit ClefGeometry(const dom::Clef& clef);

    const dom::Clef& clef() const {
        return _clef;
    }
    
private:
    const dom::Clef& _clef;
};

} // namespace mxml
