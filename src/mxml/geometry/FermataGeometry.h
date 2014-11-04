//  Created by Alejandro Isaza on 2014-06-16.
//  Copyright (c) 2014 Venture Media Labs. All rights reserved.

#pragma once
#include "MeasureElementGeometry.h"
#include <mxml/dom/Fermata.h>

namespace mxml {

class FermataGeometry : public MeasureElementGeometry {
public:
    static const Size kSize;

public:
    explicit FermataGeometry(const dom::Fermata& fermata);

    const dom::Fermata& fermata() const {
        return _fermata;
    }
    
private:
    const dom::Fermata& _fermata;
};

} // namespace
