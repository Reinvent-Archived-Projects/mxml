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
    class Iterator {
    public:
        Iterator() = default;
        Iterator(const EventSequence& eventSequence, std::size_t index);
        Iterator(const Iterator& it) = default;
        
        Iterator& operator++();
        Iterator& operator--();
        
        const Event& operator*() const;
        const Event* operator->() const;

        bool isValid() const {
            return _eventSequence;
        }
        
        const std::size_t index() const {
            return _index;
        }
        
        bool operator==(const Iterator& it) const {
            return _eventSequence == it._eventSequence && _index == it._index;
        }
        bool operator!=(const Iterator& it) const {
            return !operator==(it);
        }
        
    private:
        const EventSequence* _eventSequence;
        std::size_t _index;
    };
    
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
    
    Iterator begin() const;
    Iterator end() const;

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
