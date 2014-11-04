//  Created by Alejandro Isaza on 2014-03-04.
//  Copyright (c) 2014 Venture Media Labs Inc. All rights reserved.

#include "Measure.h"

#include <map>

namespace mxml {
namespace dom {

Measure::Measure() :
        _number(),
        _baseAttributes()
{}

time_t Measure::smallestTimeDelta() const {
    time_t minDelta = std::numeric_limits<time_t>::max();
    time_t lastTime = 0;
    for (auto& node : _nodes) {
        const TimedNode* timedNode = dynamic_cast<const TimedNode*>(node.get());
        if (!timedNode)
            continue;
        
        time_t time = timedNode->start();
        if (lastTime != 0 && time - lastTime < minDelta)
            minDelta = time - lastTime;
        lastTime = time;
    }
    
    time_t time = duration();
    if (time - lastTime < minDelta)
        minDelta = time - lastTime;
    
    return minDelta;
}

time_t Measure::duration() const {
    if (_nodes.empty())
        return 0;
    
    time_t lastEnd = 0;
    for (auto& node : _nodes) {
        const TimedNode* timedNode = dynamic_cast<const TimedNode*>(node.get());
        if (!timedNode)
            continue;
        
        time_t end = timedNode->start() + timedNode->duration();
        if (end > lastEnd)
            lastEnd = end;
    }
    
    return lastEnd;
}

} // namespace dom
} // namespace mxml
