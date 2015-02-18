//  Created by Alejandro Isaza on 2014-05-15.
//  Copyright (c) 2014 Venture Media Labs. All rights reserved.

#include "EventSequence.h"

#include <algorithm>


namespace mxml {

EventSequence::EventSequence(const ScoreProperties& scoreProperties) : _scoreProperties(scoreProperties) {
    
}

Event& EventSequence::addEvent(const Event& event) {
    auto it = std::lower_bound(_events.begin(), _events.end(), event);
    if (it != _events.end() && it->absoluteTime() == event.absoluteTime()) {
        auto& oldEvent = *it;

        // Event already exists, merge
        oldEvent.setMeasureIndex(event.measureIndex());
        oldEvent.setMeasureTime(event.measureTime());
        oldEvent.setBeatMark(oldEvent.isBeatMark() || event.isBeatMark());
        oldEvent.onNotes().insert(oldEvent.onNotes().end(), event.onNotes().begin(), event.onNotes().end());
        oldEvent.offNotes().insert(oldEvent.offNotes().end(), event.offNotes().begin(), event.offNotes().end());

        return oldEvent;
    } else {
        return *_events.insert(it, event);
    }
}

void EventSequence::clear() {
    _events.clear();
}

EventSequence::Iterator EventSequence::find(dom::time_t time) const {
    auto it = std::lower_bound(_events.begin(), _events.end(), time, [](const Event& event, dom::time_t time) {
        return event.absoluteTime() < time;
    });
    if (it == _events.end() || it->absoluteTime() != time)
        return _events.end();
    return it;
}

EventSequence::Iterator EventSequence::findClosest(dom::time_t time) const {
    auto it2 = std::lower_bound(_events.begin(), _events.end(), time, [](const Event& event, dom::time_t time) {
        return event.absoluteTime() < time;
    });
    auto it1 = std::prev(it2);
    if (it2 == _events.begin())
        return it2;
    if (it1 == _events.end())
        return it2;
    
    auto d1 = std::abs(time - it1->absoluteTime());
    auto d2 = std::abs(time - it2->absoluteTime());
    if (d1 < d2)
        return it1;
    return it2;
}

std::size_t EventSequence::index(dom::time_t time) const {
    auto it = find(time);
    if (it == _events.end())
        return static_cast<std::size_t>(-1);
    return it - _events.begin();
}

std::size_t EventSequence::measureIndex(dom::time_t time) const {
    auto e = event(time);
    if (!e)
        return static_cast<std::size_t>(-1);
    return e->measureIndex();
}

const Event* EventSequence::event(dom::time_t time) const {
    auto it = std::lower_bound(_events.begin(), _events.end(), time, [](const Event& event, dom::time_t time) {
        return event.absoluteTime() < time;
    });
    if (it == _events.end() || it->absoluteTime() != time)
        return nullptr;
    
    return &*it;
}

Event* EventSequence::event(dom::time_t time) {
    auto it = std::lower_bound(_events.begin(), _events.end(), time, [](const Event& event, dom::time_t time) {
        return event.absoluteTime() < time;
    });
    if (it == _events.end() || it->absoluteTime() != time)
        return nullptr;
    
    return &*it;
}

const Event* EventSequence::firstEvent(std::size_t measureIndex) const {
    auto it = std::find_if(_events.begin(), _events.end(), [measureIndex](const Event& event) {
        return event.measureIndex() == measureIndex;
    });
    if (it == _events.end())
        return nullptr;
    
    return &*it;
}

const Event* EventSequence::lastEvent(std::size_t measureIndex) const {
    auto it = std::find_if(_events.rbegin(), _events.rend(), [measureIndex](const Event& event) {
        return event.measureIndex() == measureIndex;
    });
    if (it == _events.rend())
        return nullptr;
    
    return &*it;
}

dom::time_t EventSequence::duration() const {
    if (_events.empty())
        return 0;

    auto& lastEvent = _events.back();
    return lastEvent.absoluteTime() + lastEvent.maxDuration();
}

EventSequence::Iterator EventSequence::begin(std::size_t measureIndex) const {
    auto it = std::lower_bound(_events.begin(), _events.end(), measureIndex, [](const Event& event1, std::size_t measureIndex) {
        return event1.measureIndex() < measureIndex;
    });
    if (it == _events.end() || it->measureIndex() != measureIndex)
        return _events.end();
    return it;
}

EventSequence::Iterator EventSequence::end(std::size_t measureIndex) const {
    return begin(measureIndex + 1);
}

} // namespace mxml
