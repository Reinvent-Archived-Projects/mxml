//  Created by Alejandro Isaza on 2014-07-22.
//  Copyright (c) 2014 Venture Media Labs. All rights reserved.

#pragma once
#include "Geometry.h"
#include <mxml/dom/Lyric.h>

namespace mxml {

class LyricGeometry : public Geometry {
public:
    explicit LyricGeometry(const dom::Lyric& lyric);

    const dom::Lyric& lyric() const {
        return _lyric;
    }

private:
    const dom::Lyric& _lyric;
};

} // namespace
