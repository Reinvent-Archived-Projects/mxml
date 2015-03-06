//  Created by Alejandro Isaza on 2014-07-22.
//  Copyright (c) 2014 Venture Media Labs. All rights reserved.

#include "LyricGeometry.h"
#include <mxml/StringUtility.h>

namespace mxml {

// Character widths for the first 256 code points of the font used to render lyrics
// See https://gist.github.com/aleph7/3f29c6da342f639b9232
static const int kCharacterHeight = 23;
static const int kCharacterDefaultWidth = 20;
static const int kCharacterWidths[256] = {
     0,  0,  0,  0,  0,  0,  0,  0,  0, 28,  0,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
     5,  5,  9, 14, 10, 18, 14,  4,  5,  5,  9, 14,  5,  7,  5, 11,
    10, 10, 10, 10, 10, 10, 10, 10, 10, 10,  5,  5, 14, 14, 14,  8,
    19, 14, 13, 15, 16, 13, 12, 16, 17,  8,  8, 16, 13, 19, 15, 17,
    12, 17, 15, 11, 14, 16, 15, 19, 16, 14, 13,  7, 11,  7, 10, 10,
     7, 10, 11,  9, 11,  9,  6, 10, 11,  5,  5, 10,  5, 17, 11, 10,
    11, 11,  8,  7,  6, 11, 10, 14, 10, 10,  9, 10, 11, 10, 14,  0,
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
     5,  5,  9, 15, 14, 13, 11,  8,  7, 16,  7, 10, 14,  0, 16, 10,
     8, 14,  7,  7,  7, 10,  9,  7,  7,  7,  7, 10, 18, 18, 18,  8,
    14, 14, 14, 14, 14, 14, 18, 15, 13, 13, 13, 13,  8,  8,  8,  8,
    16, 15, 17, 17, 17, 17, 17, 14, 17, 16, 16, 16, 16, 14, 12, 11,
    10, 10, 10, 10, 10, 10, 13,  9,  9,  9,  9,  9,  5,  5,  5,  5,
    10, 11, 10, 10, 10, 10, 10, 14, 10, 11, 11, 11, 11, 10, 11, 10, 
};


size_t utf8ToCodePoint(char32_t* pc32, const char* s, size_t n);


LyricGeometry::LyricGeometry(const dom::Lyric& lyric, int staff)
: PlacementGeometry(lyric.placement(), staff),
  _lyric(lyric)
{
    setSize({static_cast<coord_t>(width(lyric)), kCharacterHeight});

    setHorizontalAnchorPointValues(0.5, 0);
    if (lyric.placement() == dom::Placement::Above)
        setVerticalAnchorPointValues(1, 0);
    else
        setVerticalAnchorPointValues(0, 0);
}

coord_t LyricGeometry::width(const dom::Lyric& lyric) {
    const auto& size = lyric.text().length();

    auto stringCopy = lyric.text();
    const auto& string = trim(stringCopy).c_str();

    int width = 0;
    for (std::size_t i = 0; i < size; ) {
        char32_t codePoint;
        auto result = utf8ToCodePoint(&codePoint, string + i, size - i);
        if (static_cast<int>(result) < 0)
            break;

        i += result;
        if (codePoint < 255)
            width += kCharacterWidths[codePoint];
        else
            width += kCharacterDefaultWidth;
    }
    return width;
}

} // namespace
