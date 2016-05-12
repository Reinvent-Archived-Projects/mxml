// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once
#include "Point.h"

#include <initializer_list>
#include <stdexcept>

namespace mxml {

class Size {
public:
    typedef coord_t value_t;
    
public:
    Size() : width(), height() {}
    Size(value_t w, value_t h) : width(w), height(h) {}
    Size(std::initializer_list<coord_t> l) {
        if (l.size() != 2)
            throw std::invalid_argument("initializer_list should contain 2 elements.");
        
        auto it = l.begin();
        width = *it;
        ++it;
        height = *it;
    }
    
public:
    value_t width;
    value_t height;
    
public:
    bool operator==(const Size& s) const {
        return width == s.width && height == s.height;
    }
    bool operator!=(const Size& s) const {
        return width != s.width || height != s.height;
    }
};

} // namespace mxml
