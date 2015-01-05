//  Created by Alejandro Isaza on 2014-07-22.
//  Copyright (c) 2014 Venture Media Labs. All rights reserved.

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
