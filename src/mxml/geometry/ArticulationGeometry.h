//  Created by Alejandro Isaza on 2014-05-08.
//  Copyright (c) 2014 Venture Media Labs. All rights reserved.

#pragma once
#include "Geometry.h"
#include <mxml/dom/Articulation.h>

namespace mxml {

class ArticulationGeometry : public Geometry {
public:
    static const Size kAccentSize;
    static const Size kSpiccatoSize;
    static const Size kStaccatoSize;
    static const Size kStaccatissimoSize;
    
public:
    explicit ArticulationGeometry(const dom::Articulation& articulation, dom::Stem stem);
    
    const dom::Articulation& articulation() const {
        return _articulation;
    }
    const dom::Stem stem() const {
        return _stem;
    }
    
private:
    const dom::Articulation& _articulation;
    const dom::Stem _stem;
};

} // namespace mxml
