//  Created by Alejandro Isaza on 2014-06-03.
//  Copyright (c) 2014 Venture Media Labs. All rights reserved.

#pragma once
#include "Geometry.h"
#include <mxml/dom/Ornaments.h>

namespace mxml {

class OrnamentsGeometry : public Geometry {
public:
    explicit OrnamentsGeometry(const dom::Ornaments& ornaments);

    const dom::Ornaments& ornaments() const {
        return _ornaments;
    }

private:
    const dom::Ornaments& _ornaments;
};

} // namespace
