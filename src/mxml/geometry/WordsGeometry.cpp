// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "WordsGeometry.h"
#include <mxml/StringUtility.h>


namespace mxml {

// Character dimensions for the first 256 code points of the font used to render words.
// See https://gist.github.com/aleph7/3f29c6da342f639b9232
static const int kWordCharacterHeight = 23;
static const int kWordCharacterDefaultWidth = 20;
static const int kWordCharacterWidths[256] = {
     0,  0,  0,  0,  0,  0,  0,  0,  0, 28,  0,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
     5,  6, 11, 14, 10, 17, 15,  6,  6,  6, 11, 14,  6,  7,  6,  6,
    10, 10, 10, 10, 10, 10, 10, 10, 10, 10,  6,  6, 14, 14, 14,  9,
    18, 14, 14, 15, 16, 14, 12, 16, 17,  8,  8, 16, 13, 20, 16, 17,
    13, 17, 15, 12, 15, 15, 16, 20, 17, 15, 13,  8,  6,  8, 12, 10,
     7, 11, 12, 10, 13, 11,  7, 11, 12,  7,  7, 12,  7, 19, 12, 12,
    13, 12,  9, 10,  8, 12, 11, 17, 12, 11, 10,  8, 12,  8, 14,  0,
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
     5,  6, 10, 15, 14, 15, 12, 11,  7, 15,  7, 12, 14,  0, 15, 10,
     8, 14,  6,  6,  7, 12, 11,  6,  7,  6,  8, 12, 16, 16, 16,  9,
    14, 14, 14, 14, 14, 14, 20, 15, 14, 14, 14, 14,  8,  8,  8,  8,
    16, 16, 17, 17, 17, 17, 17, 14, 17, 15, 15, 15, 15, 15, 13, 13,
    11, 11, 11, 11, 11, 11, 15, 10, 11, 11, 11, 11,  7,  7,  7,  7,
    12, 12, 12, 12, 12, 12, 12, 14, 12, 12, 12, 12, 12, 11, 13, 11, 
};

// Character dimensions for the first 256 code points of the font used to render dynamics
// See https://gist.github.com/aleph7/3f29c6da342f639b9232
static const int kDynamicsCharacterHeight = 30;
static const int kDynamicsCharacterDefaultWidth = 26;
static const int kDynamicsCharacterWidths[256] = {
     0,  0,  0,  0,  0,  0,  0,  0,  0, 28,  0,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
     7,  9, 15, 18, 13, 24, 21,  8,  9,  9, 15, 18,  9,  9,  9, 17,
    14, 14, 14, 14, 14, 14, 14, 14, 14, 14,  9,  9, 18, 18, 18, 12,
    22, 18, 17, 19, 20, 18, 15, 21, 22, 11, 12, 20, 16, 26, 20, 21,
    16, 21, 19, 15, 18, 19, 17, 25, 18, 16, 17, 10, 17, 10, 15, 13,
     9, 15, 16, 13, 16, 13,  9, 14, 16,  9,  8, 16,  9, 26, 16, 15,
    16, 16, 12, 13, 10, 16, 15, 22, 15, 14, 13, 11, 16, 11, 18,  0,
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
     7,  9, 13, 19, 18, 15, 16, 13,  9, 20, 10, 15, 18,  0, 20, 13,
    11, 18,  9,  9,  9, 16, 15,  9,  9,  9, 10, 15, 24, 24, 24, 12,
    18, 18, 18, 18, 18, 18, 24, 19, 18, 18, 18, 18, 11, 11, 11, 11,
    20, 20, 21, 21, 21, 21, 21, 18, 21, 19, 19, 19, 19, 16, 16, 16,
    15, 15, 15, 15, 15, 15, 19, 13, 13, 13, 13, 13,  9,  9,  9,  9,
    15, 16, 15, 15, 15, 15, 15, 18, 15, 16, 16, 16, 16, 14, 16, 14, 
};

// Dynamics' slanted font required some extra space at the start
static const int kDynamicsExtraWidth = 10;


WordsGeometry::WordsGeometry(const dom::Direction& direction)
: PlacementGeometry(direction.placement(), direction.staff()), _direction(direction), _dynamics(false)
{
    dom::Dynamics* dynamics = dynamic_cast<dom::Dynamics*>(direction.type());
    if (dynamics) {
        setSize(computeSize(*dynamics));
        setHorizontalAnchorPointValues(0.5, 0);
        _dynamics = true;
    }
    
    dom::Words* words = dynamic_cast<dom::Words*>(direction.type());
    if (words) {
        setSize(computeSize(*words));
        setHorizontalAnchorPointValues(0, 0);
    }

    if (direction.placement() == dom::Placement::Above)
        setVerticalAnchorPointValues(1, 0);
    else
        setVerticalAnchorPointValues(0, 0);
}
    
const std::string* WordsGeometry::contents() const {
    mxml::dom::Dynamics* dynamics = dynamic_cast<mxml::dom::Dynamics*>(_direction.type());
    if (dynamics)
        return &dynamics->string();
    
    mxml::dom::Words* words = dynamic_cast<mxml::dom::Words*>(_direction.type());
    if (words)
        return &words->contents();
    
    return nullptr;
}
    
Size WordsGeometry::computeSize(const dom::Words& words) {
    auto stringCopy = words.contents();
    const auto& string = trim(stringCopy).c_str();
    const auto& length = stringCopy.length();

    Size size{0, kWordCharacterHeight};
    for (std::size_t i = 0; i < length; ) {
        char32_t codePoint;
        auto result = utf8ToCodePoint(&codePoint, string + i, length - i);
        if (static_cast<int>(result) < 0)
            break;

        i += result;
        if (codePoint <= 255)
            size.width += kWordCharacterWidths[codePoint];
        else
            size.width += kWordCharacterDefaultWidth;
    }

    return size;
}

Size WordsGeometry::computeSize(const dom::Dynamics& dynamics) {
    auto stringCopy = dynamics.string();
    const auto& string = trim(stringCopy).c_str();
    const auto& length = stringCopy.length();

    Size size{kDynamicsExtraWidth, kDynamicsCharacterHeight};
    for (std::size_t i = 0; i < length; ) {
        char32_t codePoint;
        auto result = utf8ToCodePoint(&codePoint, string + i, length - i);
        if (static_cast<int>(result) < 0)
            break;

        i += result;
        if (codePoint <= 255)
            size.width += kDynamicsCharacterWidths[codePoint];
        else
            size.width += kDynamicsCharacterDefaultWidth;
    }
    
    return size;
}

} // namespace mxml
