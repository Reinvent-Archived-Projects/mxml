//  Created by Alejandro Isaza on 2014-07-22.
//  Copyright (c) 2014 Venture Media Labs. All rights reserved.

#include "LyricGeometry.h"
#include <mxml/StringUtility.h>

namespace mxml {

// Character widths for the first 255 code points of the font used to render lyrics
static const int kCharacterWidths[255] = {
    1, 1, 1, 1, 1, 1, 1, 1, 1, 29, 6, 8, 8, 6, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 6, 6, 10, 15, 11,
    19, 15, 5, 6, 6, 10, 15, 6, 8, 6, 12, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 6, 6, 15, 15, 15, 9, 20, 15, 14, 16,
    17, 14, 13, 17, 18, 9, 9, 17, 14, 20, 16, 18, 13, 18, 16, 12, 15, 17, 16, 20, 17, 15, 14, 8, 12, 8, 11, 11, 8, 11,
    12, 10, 12, 10, 7, 11, 12, 6, 6, 11, 6, 18, 12, 11, 12, 12, 9, 8, 7, 12, 11, 15, 11, 11, 10, 11, 12, 11, 15, 1, 1,
    1, 1, 1, 1, 24, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 6, 6, 10, 16, 15, 14,
    12, 9, 8, 17, 8, 11, 15, 1, 17, 11, 9, 15, 8, 8, 8, 11, 10, 8, 8, 8, 8, 11, 19, 19, 19, 9, 15, 15, 15, 15, 15, 15,
    19, 16, 14, 14, 14, 14, 9, 9, 9, 9, 17, 16, 18, 18, 18, 18, 18, 15, 18, 17, 17, 17, 17, 15, 13, 12, 11, 11, 11, 11,
    11, 11, 14, 10, 10, 10, 10, 10, 6, 6, 6, 6, 11, 12, 11, 11, 11, 11, 11, 15, 11, 12, 12, 12, 12, 11, 12
};
static const int kDefaultCharacterWidth = 12;


size_t utf8ToCodePoint(char32_t* pc32, const char* s, size_t n);


LyricGeometry::LyricGeometry(const dom::Lyric& lyric, int staff)
: PlacementGeometry(lyric.placement(), staff),
  _lyric(lyric)
{
    setSize({static_cast<coord_t>(width(lyric)), 23});

    setHorizontalAnchorPointValues(0.5, 0);
    if (lyric.placement() == dom::kPlacementAbove)
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
            width += kDefaultCharacterWidth;
    }
    return width;
}

} // namespace
