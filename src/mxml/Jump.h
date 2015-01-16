//  Created by Alejandro Isaza on 2015-01-14.
//  Copyright (c) 2015 Venture Media Labs. All rights reserved.

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
