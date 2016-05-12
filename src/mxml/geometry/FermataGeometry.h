// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

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
