// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once
#include "Event.h"
#include <mxml/dom/Sound.h>
#include <mxml/ScoreProperties.h>

#include <set>
#include <vector>

namespace mxml {

class EventSequence {
public:
    using ConstIterator = std::vector<Event>::const_iterator;
    using Iterator = std::vector<Event>::iterator;
    
public:
    EventSequence(const ScoreProperties& scoreProperties);
    
    Event& addEvent(const Event& event);
    void clear();

    const ScoreProperties& scoreProperties() const {
        return _scoreProperties;
    }

    /**
     Get the start time of the event sequence, in abosulte time.
     */
    dom::time_t startTime() const;

    /**
     Get the end time of the event sequence, in abosulte time.
     */
    dom::time_t endTime() const;

    const std::vector<Event>& events() const {
        return _events;
    }
    std::vector<Event>& events() {
        return _events;
    }

    ConstIterator begin() const {
        return _events.begin();
    }
    Iterator begin() {
        return _events.begin();
    }
    ConstIterator end() const {
        return _events.end();
    }
    Iterator end() {
        return _events.end();
    }

    /**
     Find the event at the given absolute time
     */
    ConstIterator find(dom::time_t time) const;

    /**
     Find the event at the given absolute time
     */
    Iterator find(dom::time_t time);

    /**
     Find the event at the given measure location
     */
    ConstIterator findClosest(MeasureLocation measureLocation) const;

    /**
     Find the closest event to the given absolute time.
     */
    ConstIterator findClosest(dom::time_t time) const;

    /**
     Find the closest event to the given absolute time.
     */
    Iterator findClosest(dom::time_t time);

    ConstIterator firstInMeasure(std::size_t measureIndex) const;

private:
    const ScoreProperties& _scoreProperties;
    std::vector<Event> _events;

    friend class EventFactory;
};

} // namespace mxml
