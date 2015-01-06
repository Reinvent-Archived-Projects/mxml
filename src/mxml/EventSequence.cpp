//  Created by Alejandro Isaza on 2014-05-15.
//  Copyright (c) 2014 Venture Media Labs. All rights reserved.

#include "EventSequence.h"

#include <algorithm>

namespace mxml {

EventSequence::Iterator::Iterator(const EventSequence& eventSequence, std::size_t index, dom::time_t time, int loopNumber)
: _eventSequence(&eventSequence), _index(index), _time(time), _loopNumber(loopNumber)
{
}

EventSequence::Iterator& EventSequence::Iterator::operator++() {
    std::size_t prevIndex = _index;
    dom::time_t prevTime = _time;
    
    _index += 1;
    updateTime();
    
    updateNext(prevTime, prevIndex);
    
    return *this;
}

EventSequence::Iterator& EventSequence::Iterator::operator--() {
    std::size_t prevIndex = _index;
    dom::time_t prevTime = _time;

    _index -= 1;
    updateTime();

    updatePrev(prevTime, prevIndex);
    
    return *this;
}

void EventSequence::Iterator::updateTime() {
    if (_index < _eventSequence->events().size())
        _time = _eventSequence->events().at(_index).time();
}

void EventSequence::Iterator::updatePrev(dom::time_t prevTime, std::size_t prevIndex) {
    if (_index >= _eventSequence->events().size()) {
        *this = _eventSequence->end();
        return;
    }

    const Loop* prevLoop = _eventSequence->loop(prevTime);
    const Loop* loop = _eventSequence->loop(_time);

    // Loop
    if (prevLoop && prevLoop != loop && _loopNumber > 1) {
        _loopNumber -= 1;
        _index = _eventSequence->index(prevLoop->end - 1);
        updateTime();
        updatePrev(prevTime, prevIndex);
        return;
    }

    const Ending* ending = _eventSequence->ending(_time);

    // Skip ending
    if (ending && ending->numbers.count(_loopNumber) == 0) {
        _index = _eventSequence->index(ending->begin - 1);
        updateTime();
        updatePrev(prevTime, prevIndex);
        return;
    }
    
    // Start new loop
    if (loop && prevLoop != loop) {
        _loopNumber = loop->count;
    }
}

void EventSequence::Iterator::updateNext(dom::time_t prevTime, std::size_t prevIndex) {
    if (_index >= _eventSequence->events().size()) {
        *this = _eventSequence->end();
        return;
    }
    
    const Loop* prevLoop = _eventSequence->loop(prevTime);
    const Loop* loop = _eventSequence->loop(_time);
    
    // Loop
    if (prevLoop && prevLoop != loop && _loopNumber <= prevLoop->count) {
        _loopNumber += 1;
        _index = _eventSequence->index(prevLoop->begin);
        updateTime();
        updateNext(prevTime, prevIndex);
        return;
    }
    
    const Ending* ending = _eventSequence->ending(_time);
    
    // Skip ending
    if (ending && ending->numbers.count(_loopNumber) == 0) {
        _index = _eventSequence->index(ending->end);
        updateTime();
        updateNext(prevTime, prevIndex);
        return;
    }
    
    // Start new loop
    if (loop && prevLoop != loop) {
        _loopNumber = 1;
    }
}

const Event& EventSequence::Iterator::operator*() const {
    return _eventSequence->events().at(_index);
}

const Event* EventSequence::Iterator::operator->() const {
    return &_eventSequence->events().at(_index);
}

//const dom::Attributes* EventSequence::Iterator::attributes() const {
//    return _eventSequence->attributes(_time);
//}

float EventSequence::Iterator::tempo() const {
    const float* val = _eventSequence->tempo(_time);
    if (!val)
        return 60;
    return *val;
}

float EventSequence::Iterator::dynamics() const {
    const float* val = _eventSequence->dynamics(_time);
    if (!val)
        return 100;
    return *val;
}


EventSequence::EventSequence() {
    
}

Event& EventSequence::addEvent(const Event& event) {
    auto it = std::lower_bound(_events.begin(), _events.end(), event);
    
    // There should be no overlapping events
    assert(it == _events.end() || it->time() > event.time());
    
    return *_events.insert(it, event);
}

void EventSequence::addTempo(const Value& value) {
    auto it = std::upper_bound(_tempos.begin(), _tempos.end(), value);
    _tempos.insert(it, value);
}

void EventSequence::addDynamics(const Value& value) {
    auto it = std::upper_bound(_dynamics.begin(), _dynamics.end(), value);
    _dynamics.insert(it, value);
}

void EventSequence::addEnding(const Ending& ending) {
    assert(ending.begin < ending.end);
    auto it = std::upper_bound(_endings.begin(), _endings.end(), ending);
    _endings.insert(it, ending);
}

void EventSequence::addLoop(const Loop& loop) {
    assert(loop.begin < loop.end);
    auto it = std::upper_bound(_loops.begin(), _loops.end(), loop);
    _loops.insert(it, loop);
}

void EventSequence::clear() {
    _events.clear();
    _tempos.clear();
    _dynamics.clear();
    _loops.clear();
    _endings.clear();
}

std::size_t EventSequence::index(dom::time_t time) const {
    auto it = std::lower_bound(_events.begin(), _events.end(), time, [](const Event& event, dom::time_t time) {
        return event.time() < time;
    });
    if (it == _events.end() || it->time() != time)
        return static_cast<std::size_t>(-1);
    
    return it - _events.begin();
}

const Event* EventSequence::event(dom::time_t time) const {
    auto it = std::lower_bound(_events.begin(), _events.end(), time, [](const Event& event, dom::time_t time) {
        return event.time() < time;
    });
    if (it == _events.end() || it->time() != time)
        return nullptr;
    
    return &*it;
}

Event* EventSequence::event(dom::time_t time) {
    auto it = std::lower_bound(_events.begin(), _events.end(), time, [](const Event& event, dom::time_t time) {
        return event.time() < time;
    });
    if (it == _events.end() || it->time() != time)
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

const float* EventSequence::tempo(dom::time_t time) const {
    auto it = std::find_if(_tempos.rbegin(), _tempos.rend(), [time](const Value& value) {
        return time >= value.begin;
    });
    if (it != _tempos.rend())
        return &it->value;
    return 0;
}

const float* EventSequence::dynamics(dom::time_t time) const {
    auto it = std::find_if(_dynamics.rbegin(), _dynamics.rend(), [time](const Value& value) {
        return time >= value.begin;
    });
    if (it != _dynamics.rend())
        return &it->value;
    return 0;
}

const EventSequence::Loop* EventSequence::loop(dom::time_t time) const {
    auto it = std::find_if(_loops.begin(), _loops.end(), [time](const Loop& loop) {
        return time >= loop.begin && time < loop.end;
    });
    if (it != _loops.end())
        return &*it;
    return 0;
}

const EventSequence::Ending* EventSequence::ending(dom::time_t time) const {
    auto it = std::find_if(_endings.begin(), _endings.end(), [time](const Ending& ending) {
        return time >= ending.begin && time < ending.end;
    });
    if (it != _endings.end())
        return &*it;
    return 0;
}

EventSequence::Iterator EventSequence::begin() const {
    return Iterator(*this, 0, 0, 1);
}

EventSequence::Iterator EventSequence::end() const {
    return Iterator(*this, _events.size(), 0, 1);
}

EventSequence::Iterator EventSequence::begin(std::size_t measureIndex) const {
    return Iterator(*this, 0, 0, 1);
}

EventSequence::Iterator EventSequence::end(std::size_t measureIndex) const {
    return Iterator(*this, _events.size(), 0, 1);
}

} // namespace mxml
