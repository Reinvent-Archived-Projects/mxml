//  Created by Alejandro Isaza on 2014-04-29.
//  Copyright (c) 2014 Venture Media Labs. All rights reserved.

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
