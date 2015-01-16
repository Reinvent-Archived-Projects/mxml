//  Created by Alejandro Isaza on 2014-05-15.
//  Copyright (c) 2014 Venture Media Labs. All rights reserved.

#include "EventSequence.h"

#include <algorithm>


namespace mxml {

EventSequence::Iterator::Iterator(const EventSequence& eventSequence, std::size_t index)
: _eventSequence(&eventSequence), _index(index)
{
}

EventSequence::Iterator& EventSequence::Iterator::operator++() {
    _index += 1;
    if (_index > _eventSequence->events().size())
        _index = _eventSequence->events().size();
    return *this;
}

EventSequence::Iterator& EventSequence::Iterator::operator--() {
    _index -= 1;
    if (_index > _eventSequence->events().size())
        _index = _eventSequence->events().size();
    return *this;
}

const Event& EventSequence::Iterator::operator*() const {
    return _eventSequence->events().at(_index);
}

const Event* EventSequence::Iterator::operator->() const {
    return &_eventSequence->events().at(_index);
}


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

std::size_t EventSequence::index(dom::time_t time) const {
    auto it = std::lower_bound(_events.begin(), _events.end(), time, [](const Event& event, dom::time_t time) {
        return event.absoluteTime() < time;
    });
    if (it == _events.end() || it->absoluteTime() != time)
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

EventSequence::Iterator EventSequence::begin() const {
    return Iterator(*this, 0);
}

EventSequence::Iterator EventSequence::end() const {
    return Iterator(*this, _events.size());
}

EventSequence::Iterator EventSequence::begin(std::size_t measureIndex) const {
    auto it = std::find_if(_events.begin(), _events.end(), [measureIndex](const Event& event) {
        return event.measureIndex() == measureIndex;
    });
    if (it == _events.end())
        return end();
    
    return Iterator(*this, it - _events.begin());
}

EventSequence::Iterator EventSequence::end(std::size_t measureIndex) const {
    return begin(measureIndex + 1);
}

} // namespace mxml
