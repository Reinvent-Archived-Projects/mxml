//  Created by Alejandro Isaza on 2014-05-05.
//  Copyright (c) 2014 Venture Media Labs. All rights reserved.

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
