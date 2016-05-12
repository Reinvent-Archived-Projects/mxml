// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once
#include "PlacementGeometry.h"
#include <mxml/dom/Ornaments.h>

namespace mxml {

class OrnamentsGeometry : public PlacementGeometry {
public:
    explicit OrnamentsGeometry(const dom::Ornaments& ornaments, int staff);

    const dom::Ornaments& ornaments() const {
        return _ornaments;
    }

private:
    const dom::Ornaments& _ornaments;
};

} // namespace
