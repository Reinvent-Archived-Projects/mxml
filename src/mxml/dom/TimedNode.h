//  Created by Alejandro Isaza on 2/25/2014.
//  Copyright (c) 2014 Venture Media Labs Inc. All rights reserved.

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
