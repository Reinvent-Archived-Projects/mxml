// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once
#include <string>

namespace mxml {
    /**
     Convert a sequence of UTF8 bytes into a unicode code point.

     @param pc32 On return will be filled with the code point
     @param s    The string of UTF8 bytes.
     @param n    The maximum number of bytes to analyze.

     @return The number of bytes consumed or a negative number to denote an error.
     */
    std::size_t utf8ToCodePoint(char32_t* pc32, const char* s, std::size_t n);

    std::string& ltrim(std::string& s);
    std::string& rtrim(std::string& s);
    std::string& trim(std::string& s);
}
