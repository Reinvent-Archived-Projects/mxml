//  Created by Alejandro Isaza on 2014-07-22.
//  Copyright (c) 2014 Venture Media Labs. All rights reserved.

#include "LyricGeometry.h"

namespace mxml {

LyricGeometry::LyricGeometry(const dom::Lyric& lyric) : _lyric(lyric) {
    setSize({static_cast<coord_t>(20 * lyric.text().length()), 23});

    setHorizontalAnchorPointValues(0.5, 0);
    if (lyric.placement() == dom::PLACEMENT_ABOVE)
        setVerticalAnchorPointValues(1, 0);
    else
        setVerticalAnchorPointValues(0, 0);
}

} // namespace
