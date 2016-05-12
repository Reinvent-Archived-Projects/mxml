// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once
#include "PlacementGeometry.h"
#include <mxml/dom/Lyric.h>

namespace mxml {

class LyricGeometry : public PlacementGeometry {
public:
    explicit LyricGeometry(const dom::Lyric& lyric, int staff);

    const dom::Lyric& lyric() const {
        return _lyric;
    }

    static coord_t width(const dom::Lyric& lyric);

private:
    const dom::Lyric& _lyric;
};

} // namespace
