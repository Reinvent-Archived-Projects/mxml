//  Created by Alejandro Isaza on 2014-05-15.
//  Copyright (c) 2014 Venture Media Labs. All rights reserved.

#pragma once
#include "Event.h"
#include <mxml/dom/Sound.h>
#include <mxml/ScoreProperties.h>

#include <set>
#include <vector>

namespace mxml {

class EventSequence {
public:
    using Iterator = std::vector<Event>::const_iterator;
    
public:
    EventSequence(const ScoreProperties& scoreProperties);
    
    Event& addEvent(const Event& event);
    void clear();
    
    const std::vector<Event>& events() const {
        return _events;
    }
    std::vector<Event>& events() {
        return _events;
    }

    /**
     Find the event at the given absolute time
     */
    Iterator find(dom::time_t time) const;

    /**
     Find the closest event to the given absolute time.
     */
    Iterator findClosest(dom::time_t time) const;
    
    /** Return the index of the event at the given absolute time, -1 if there is no such event.
     */
    std::size_t index(dom::time_t time) const;

    /**
     Return the measure index of the event at the given absolute time, -1 if there is no such event.
     */
    std::size_t measureIndex(dom::time_t time) const;
    
    /** Return a constant pointer to the event at the given absolute time, 0 if there is no such event.
     */
    const Event* event(dom::time_t time) const;
    
    /** Return a pointer to the event at the given absolute time, 0 if there is no such event.
     */
    Event* event(dom::time_t time);

    /** Return the first event for a given measure index.
     */
    const Event* firstEvent(std::size_t measureIndex) const;

    /** Return the last event for a given measure index.
     */
    const Event* lastEvent(std::size_t measureIndex) const;

    /**
     Get the total duration of the event sequence, in abosulte time.
     */
    dom::time_t duration() const;

    Iterator begin() const {
        return _events.begin();
    }
    Iterator end() const {
        return _events.end();
    }

    Iterator begin(std::size_t measureIndex) const;
    Iterator end(std::size_t measureIndex) const;

    const ScoreProperties& scoreProperties() const {
        return _scoreProperties;
    }

private:
    const ScoreProperties& _scoreProperties;
    std::vector<Event> _events;

    friend class EventFactory;
};

} // namespace mxml
