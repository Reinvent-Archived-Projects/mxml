// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "StringUtility.h"

#include <algorithm>
#include <functional>
#include <cctype>

namespace mxml {
    size_t utf8ToCodePoint(char32_t* pc32, const char* s, size_t n) {
        // first case: null character
        if (*s == 0) {
            *pc32 = 0;
            return 0;
        }

        // second case: simple ASCII char
        if ((*s & 0x80) == 0) {
            *pc32 = *s;
            return 1;
        }

        // last case: multibytes characters
        if ((*s & 0xE0) == 0xC0) {                              // 110xxxxx => 2 bytes
            if (n < 2)
                return static_cast<std::size_t>(-2);
            *pc32 = ((s[0] & 0x1F) << 6) | (s[1] & 0x3F);
            return 2;
        } else if ((*s & 0xF0) == 0xE0) {                       // 1110xxxx => 3 bytes
            if (n < 3)
                return static_cast<std::size_t>(-2);
            *pc32 = ((s[0] & 0xF) << 12) | ((s[1] & 0x3F) << 6) | (s[2] & 0x3F);
            return 3;
        } else if ((*s & 0xF8) == 0xF0) {                       // 11110xxx => 4 bytes
            if (n < 4)
                return static_cast<std::size_t>(-2);
            *pc32 = ((s[0] & 0x7) << 18) | ((s[1] & 0x3F) << 12) | ((s[2] & 0x3F) << 6) | (s[4] & 0x3F);
            return 4;
        }
        
        return static_cast<std::size_t>(-1);
    }

    std::string& ltrim(std::string& s) {
        const auto& end = std::find_if(s.begin(), s.end(), [](char c) {
            return !std::isspace(c);
        });
        s.erase(s.begin(), end);
        return s;
    }

    std::string& rtrim(std::string& s) {
        const auto& begin = std::find_if(s.rbegin(), s.rend(), [](char c) {
            return !std::isspace(c);
        }).base();
        s.erase(begin, s.end());
        return s;
    }

    std::string& trim(std::string& s) {
        return ltrim(rtrim(s));
    }
}