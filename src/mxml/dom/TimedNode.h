// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once
#include "Node.h"
#include "Optional.h"
#include "Types.h"

namespace mxml {
namespace dom {

class TimedNode : public Node {
public:
    TimedNode() : _start(), _duration() {}
    virtual ~TimedNode() {}
    
    time_t start() const {
        return _start;
    }
    void setStart(time_t start) {
        _start = start;
    }
    
    const Optional<time_t>& duration() const {
        return _duration;
    }
    Optional<time_t>& duration() {
        return _duration;
    }
    void setDuration(const Optional<time_t>& duration) {
        _duration = duration;
    }
    
private:
    time_t _start;
    Optional<time_t> _duration;
};

} // namespace dom
} // namespace mxml
