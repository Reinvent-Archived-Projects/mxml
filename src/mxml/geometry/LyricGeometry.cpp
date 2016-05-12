// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "LyricGeometry.h"
#include <mxml/StringUtility.h>

namespace mxml {

static const std::size_t kLyricPadding = 2;

// Character widths for the first 256 code points of the font used to render lyrics
// See https://gist.github.com/aleph7/3f29c6da342f639b9232
static const int kCharacterHeight = 21;
static const int kCharacterDefaultWidth = 18;
static const int kCharacterWidths[256] = {
    0,  0,  0,  0,  0,  0,  0,  0,  0, 28,  0,  6,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    5,  5,  8, 12,  9, 16, 13,  4,  5,  5,  8, 12,  5,  6,  5, 10,
    9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  5,  5, 12, 12, 12,  8,
    17, 13, 12, 13, 14, 12, 10, 14, 15,  7,  7, 15, 11, 17, 14, 15,
    11, 15, 14, 10, 13, 14, 14, 18, 14, 13, 12,  7, 10,  7,  9,  9,
    7,  9, 10,  8, 10,  8,  5,  9, 10,  5,  5,  9,  5, 15, 10,  9,
    10, 10,  7,  7,  6, 10,  9, 13,  9,  9,  8,  9, 10,  9, 12,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    10, 10,  7,  7,  6, 10,  9, 13,  9,  9,  8,  9, 10,  9, 12,  0,
    5,  5,  8, 13, 12, 11, 10,  8,  7, 14,  6,  9, 12,  0, 14,  9,
    8, 12,  6,  6,  7,  9,  9,  7,  7,  6,  6,  9, 16, 16, 16,  8,
    13, 13, 13, 13, 13, 13, 16, 13, 12, 12, 12, 12,  7,  7,  7,  7,
    14, 14, 15, 15, 15, 15, 15, 12, 15, 14, 14, 14, 14, 13, 11, 10,
    9,  9,  9,  9,  9,  9, 12,  8,  8,  8,  8,  8,  5,  5,  5,  5,
    9, 10,  9,  9,  9,  9,  9, 12,  9, 10, 10, 10, 10,  9, 10,  9, 
};


size_t utf8ToCodePoint(char32_t* pc32, const char* s, size_t n);


LyricGeometry::LyricGeometry(const dom::Lyric& lyric, int staff)
: PlacementGeometry(lyric.placement(), staff),
  _lyric(lyric)
{
    setSize({static_cast<coord_t>(width(lyric)), kCharacterHeight + kLyricPadding});

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
    return width + kLyricPadding;
}

} // namespace
