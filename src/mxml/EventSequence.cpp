// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

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

dom::time_t EventSequence::startTime() const {
    if (_events.empty())
        return 0;

    auto& firstEvent = _events.front();
    return firstEvent.absoluteTime();
}

dom::time_t EventSequence::endTime() const {
    if (_events.empty())
        return 0;

    auto& lastEvent = _events.back();
    return lastEvent.absoluteTime() + lastEvent.maxDuration();
}

EventSequence::ConstIterator EventSequence::find(dom::time_t time) const {
    auto it = std::lower_bound(_events.begin(), _events.end(), time, [](const Event& event, dom::time_t time) {
        return event.absoluteTime() < time;
    });
    if (it == _events.end() || it->absoluteTime() != time)
        return _events.end();
    return it;
}

EventSequence::Iterator EventSequence::find(dom::time_t time) {
    auto it = std::lower_bound(_events.begin(), _events.end(), time, [](const Event& event, dom::time_t time) {
        return event.absoluteTime() < time;
    });
    if (it == _events.end() || it->absoluteTime() != time)
        return _events.end();
    return it;
}


EventSequence::ConstIterator EventSequence::findClosest(MeasureLocation measureLocation) const {
    auto it2 = std::lower_bound(_events.begin(), _events.end(), measureLocation, [](const Event& event, MeasureLocation measureLocation) {
        return event.measureLocation() < measureLocation;
    });
    if (it2->measureLocation() == measureLocation || it2 == _events.begin())
        return it2;

    auto it1 = std::prev(it2);
    if (it1 == _events.end())
        return it2;
    return it1;
}

EventSequence::ConstIterator EventSequence::findClosest(dom::time_t time) const {
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

EventSequence::Iterator EventSequence::findClosest(dom::time_t time) {
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

EventSequence::ConstIterator EventSequence::firstInMeasure(std::size_t measureIndex) const {
    return std::find_if(_events.begin(), _events.end(), [=](const Event& event) {
        return event.measureIndex() == measureIndex;
    });
}

} // namespace mxml
