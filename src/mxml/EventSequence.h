//  Created by Alejandro Isaza on 2014-05-15.
//  Copyright (c) 2014 Venture Media Labs. All rights reserved.

#pragma once
#include "Event.h"
#include <mxml/dom/Sound.h>

#include <set>
#include <vector>

namespace mxml {

class EventSequence {
public:
    struct Ending {
        dom::time_t begin;
        dom::time_t end;
        std::set<int> numbers;
        
        bool operator<(const Ending& rhs) const {
            return begin < rhs.begin;
        }
    };
    
    struct Loop {
        dom::time_t begin;
        dom::time_t end;
        int count;
        
        bool operator<(const Loop& rhs) const {
            return begin < rhs.begin;
        }
    };
    
    struct Value {
        dom::time_t begin;
        std::set<int> numbers;
        const dom::Part* part;
        float value;
        
        bool operator<(const Value& rhs) const {
            return begin < rhs.begin;
        }
    };
    
    struct Attributes {
        dom::time_t begin;
        const dom::Part* part;
        const dom::Attributes* attributes;
        
        bool operator<(const Attributes& rhs) const {
            return begin < rhs.begin;
        }
    };
    
    class Iterator {
    public:
        Iterator() = default;
        Iterator(const EventSequence& eventSequence, std::size_t index, dom::time_t time, int loopNumber);
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
        const dom::time_t time() const {
            return _time;
        }
        const int loopNumber() const {
            return _loopNumber;
        }
        
//        const dom::Attributes* attributes() const;
        float tempo() const;
        float dynamics() const;
        
        bool operator==(const Iterator& it) const {
            return _eventSequence == it._eventSequence && _index == it._index && _loopNumber == it._loopNumber;
        }
        bool operator!=(const Iterator& it) const {
            return !operator==(it);
        }
        
    private:
        void updateTime();
        void updatePrev(dom::time_t prevTime, std::size_t prevIndex);
        void updateNext(dom::time_t prevTime, std::size_t prevIndex);
        
    private:
        const EventSequence* _eventSequence;
        std::size_t _index;
        dom::time_t _time;
        int _loopNumber;
    };
    
public:
    EventSequence();
    
    Event& addEvent(const Event& event);
    void addAttributes(const Attributes& attrs);
    void addTempo(const Value& value);
    void addDynamics(const Value& value);
    void addEnding(const Ending& ending);
    void addLoop(const Loop& loop);
    void clear();
    
    const std::vector<Event>& events() const {
        return _events;
    }
    std::vector<Event>& events() {
        return _events;
    }
    const std::vector<Attributes>& attributes() const {
        return _attributes;
    }
    std::vector<Attributes>& attributes() {
        return _attributes;
    }
    const std::vector<Value>& tempos() const {
        return _tempos;
    }
    std::vector<Value>& tempos() {
        return _tempos;
    }
    const std::vector<Value>& dynamics() const {
        return _dynamics;
    }
    std::vector<Value>& dynamics() {
        return _dynamics;
    }
    const std::vector<Ending>& endings() const {
        return _endings;
    }
    std::vector<Ending>& endings() {
        return _endings;
    }
    const std::vector<Loop>& loops() const {
        return _loops;
    }
    std::vector<Loop>& loops() {
        return _loops;
    }
    
    /** Return the index of the event at the given time, -1 if there is no such event.
     */
    std::size_t index(dom::time_t time) const;
    
    /** Return a constant pointer to the event at the given time, 0 if there is no such event.
     */
    const Event* event(dom::time_t time) const;
    
    /** Return a pointer to the event at the given time, 0 if there is no such event.
     */
    Event* event(dom::time_t time);

    /** Return the first event for a given measure index.
     */
    const Event* firstEvent(std::size_t measureIndex) const;

    /** Return the last event for a given measure index.
     */
    const Event* lastEvent(std::size_t measureIndex) const;

    /** Return a pointer to the attributes for the given event time, 0 if there are no attributes defined.
     */
    const dom::Attributes* attributes(dom::time_t time) const;
    
    /** Return a pointer to the tempo for the given event time, 0 if there is no tempo defined.
     */
    const float* tempo(dom::time_t time) const;
    
    /** Return a pointer to the dynamics for the given event time, 0 if there are no dynamics defined.
     */
    const float* dynamics(dom::time_t time) const;
    
    /** Return the loop contatining the given event time, 0 if the event at that index is not part of a loop.
     */
    const Loop* loop(dom::time_t time) const;
    
    /** Return the ending containing the given event time, 0 if the event at that index is not part of an
     ending.
     */
    const Ending* ending(dom::time_t time) const;
    
    Iterator begin() const;
    Iterator end() const;

    Iterator begin(std::size_t measureIndex) const;
    Iterator end(std::size_t measureIndex) const;
    
private:
    std::vector<Event> _events;
    std::vector<Attributes> _attributes;
    std::vector<Value> _tempos;
    std::vector<Value> _dynamics;
    std::vector<Ending> _endings;
    std::vector<Loop> _loops;
};

} // namespace mxml
