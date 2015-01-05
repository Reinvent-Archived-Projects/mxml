//  Created by Alejandro Isaza on 2014-05-15.
//  Copyright (c) 2014 Venture Media Labs. All rights reserved.

#pragma once
#include "EventSequence.h"
#include "AttributesManager.h"

#include <map>

namespace mxml {

namespace dom {
    class Barline;
    class Chord;
    class Direction;
    class Measure;
    class TimedNode;
}

class EventFactory {
public:
    explicit EventFactory(const dom::Score& score);

    const EventSequence& build();
    
private:
    void processMeasure(const dom::Measure& measure);
    void processBarline(const dom::Barline& node);
    void processAttributes(const dom::Attributes& node);
    void processDirection(const dom::Direction& node);
    void processTimedNode(const dom::TimedNode& node);
    void processChord(const dom::Chord& chord);
    void addNote(const dom::Note& note);

private:
    const dom::Score& _score;
    const dom::Part* _part;

    std::size_t _measureIndex;
    dom::time_t _measureStartTime;
    dom::time_t _time;
    bool _firstPass;

    dom::time_t _loopBegin;
    dom::time_t _endingBegin;
    EventSequence _eventSequence;
    AttributesManager _attributesManager;
};

} // namespace mxml
