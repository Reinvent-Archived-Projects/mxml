// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once
#include <cstdlib>

namespace mxml {

struct Jump {
    std::size_t from;
    std::size_t to;

    Jump() = default;
    Jump(std::size_t from, std::size_t to) : from(from), to(to) {}

    bool forward() const {
        return to > from;
    }
    
    bool operator==(const Jump& rhs) const {
        return from == rhs.from && to == rhs.to;
    }

    bool operator<(const Jump& rhs) const {
        if (from < rhs.from)
            return true;
        if (from > rhs.from)
            return false;

        return to < rhs.to;
    }
};

}
