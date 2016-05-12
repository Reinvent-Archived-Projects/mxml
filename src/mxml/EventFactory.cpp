// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "EventFactory.h"

#include <mxml/dom/Backup.h>
#include <mxml/dom/Barline.h>
#include <mxml/dom/Chord.h>
#include <mxml/dom/Direction.h>
#include <mxml/dom/Forward.h>
#include <mxml/dom/Part.h>
#include <mxml/dom/TimedNode.h>
#include <mxml/dom/Types.h>

namespace mxml {

using namespace dom;

EventFactory::EventFactory(const dom::Score& score, const ScoreProperties& scoreProperties)
: _score(score),
  _scoreProperties(scoreProperties),
  _startTime(),
  _time(0)
{}

std::unique_ptr<EventSequence> EventFactory::build() {
    return build(0, 0, _score.parts().at(0)->measures().size());
}

std::unique_ptr<EventSequence> EventFactory::build(dom::time_t startTime, std::size_t startMeasureIndex, std::size_t endMeasureIndex) {
    _startTime = startTime;
    _startMeasureIndex = startMeasureIndex;
    _endMeasureIndex = endMeasureIndex;

    for (auto& part : _score.parts()) {
        _part = part.get();
        _measureStartTime = 0;
        _time = _startTime;
        for (std::size_t measureIndex = _startMeasureIndex; measureIndex < _endMeasureIndex; measureIndex += 1) {
            const Measure& measure = *part->measures().at(measureIndex);
            processMeasure(measure);
        }
    }

    setBeatMarks();
    auto eventSequence = unroll();
    fillWallTimes(*eventSequence);

    return eventSequence;
}

void EventFactory::processMeasure(const dom::Measure& measure) {
    for (auto& node : measure.nodes()) {
        if (const TimedNode* timedNode = dynamic_cast<const TimedNode*>(node.get())) {
            processTimedNode(*timedNode);
        }
    }

    auto nextTime = _measureStartTime + _scoreProperties.divisionsPerMeasure(measure.index());
    if (_time > nextTime)
        _time = nextTime;
    _measureStartTime = _time;
}

void EventFactory::processTimedNode(const TimedNode& node) {
    if (auto chord = dynamic_cast<const Chord*>(&node)) {
        processChord(*chord);
        _time += node.duration();
    } else if (auto note = dynamic_cast<const Note*>(&node)) {
        addNote(*note);
        _time += node.duration();
    } else if (dynamic_cast<const Forward*>(&node)) {
        _time += node.duration();
    } else if (dynamic_cast<const Backup*>(&node)) {
        _time -= node.duration();
    }
}

void EventFactory::processChord(const Chord& chord) {
    for (auto& note : chord.notes())
        addNote(*note);
}

void EventFactory::addNote(const Note& note) {
    auto measureIndex = note.measure()->index();

    auto& onEvent = event(measureIndex, note.start(), _time);
    if (!isTieStop(note))
        onEvent.addOnNote(note);

    auto& offEvent = event(measureIndex, note.start() + note.duration(), _time + note.duration());
    if (!isTieStart(note))
        offEvent.addOffNote(note);
}

Event& EventFactory::event(std::size_t measureIndex, dom::time_t measureTime, dom::time_t absoluteTime) {
    auto it = _events.find(std::make_pair(measureIndex, measureTime));
    if (it != _events.end())
        return it->second;

    auto event = Event{_score};
    event.setMeasureIndex(measureIndex);
    event.setMeasureTime(measureTime);
    event.setAbsoluteTime(absoluteTime);
    it = _events.insert(std::make_pair(std::make_pair(measureIndex, measureTime), event)).first;
    return it->second;
}

void EventFactory::setBeatMarks() {
    dom::time_t absoluteTime = 0;
    for (std::size_t measureIndex = _startMeasureIndex; measureIndex < _endMeasureIndex; measureIndex += 1) {
        auto divisionsPerBeat = _scoreProperties.divisionsPerBeat(measureIndex);

        dom::time_t lastTime = 0;
        auto begin = _events.lower_bound({measureIndex, 0});
        for (auto it = begin; it != _events.end(); ++it) {
            auto index = it->first.first;
            auto time = it->first.second;
            if (index != measureIndex)
                break;
            if (time > lastTime)
                lastTime = time;
        }

        for (dom::time_t time = 0; time < lastTime; time += divisionsPerBeat) {
            auto& e = event(measureIndex, time, absoluteTime);
            e.setBeatMark(true);
            absoluteTime += divisionsPerBeat;
        }
    }
}

std::unique_ptr<EventSequence> EventFactory::unroll() {
    auto eventSequence = std::unique_ptr<EventSequence>(new EventSequence(_scoreProperties));

    std::size_t measureIndex = _startMeasureIndex;
    dom::time_t measureStartTime = 0;
    dom::time_t time = 0;

    std::map<Loop, std::size_t> loopCounts;
    
    enum JumpState { kUnencountered, kFlagged, kActive, kPerformed };
    std::map<Jump, JumpState> jumpStates;

    // Iterate until we equal the measure count for loops
    // that 'end' past the last measure
    while (measureIndex <= _endMeasureIndex) {
        bool skipped = false;

        auto prevLoop = _scoreProperties.loop(measureIndex - 1);
        if (prevLoop && prevLoop->end() == measureIndex) {
            auto& loopInteration = loopCounts[*prevLoop];

            if (loopInteration < prevLoop->count()) {
                measureIndex = prevLoop->begin();
                skipped = true;
            }

            loopInteration += 1;
        }

        auto loop = _scoreProperties.loop(measureIndex);
        if (!skipped && loop) {
            auto& loopInteration = loopCounts[*loop];

            if (loop->isSkipped(loopInteration, measureIndex)) {
                measureIndex += 1;
                skipped = true;
            }
        }

        auto jumps = _scoreProperties.jumps(measureIndex);
        for (auto& jump : jumps) {
            auto& state = jumpStates[jump];
            if (jump.forward()) {
                if (state == kActive) {
                    measureIndex = jump.to;
                    skipped = true;
                    state = kPerformed;
                } else {
                    state = kFlagged;
                }
            } else if (!jump.forward()) {
                if (state == kUnencountered) {
                    measureIndex = jump.to;
                    skipped = true;
                    for (auto& pair : jumpStates) {
                        if (pair.second == kFlagged)
                            pair.second = kActive;
                    }
                }
                state = kPerformed;
            }
        }

        if (!skipped) {
            auto measureDuration = _scoreProperties.divisionsPerMeasure(measureIndex);
            auto first = _events.lower_bound(std::make_pair(measureIndex, 0));
            auto second = _events.upper_bound(std::make_pair(measureIndex, measureDuration));
            for (auto it = first; it != second; ++it) {
                auto event = it->second;
                time = measureStartTime + event.measureTime();
                event.setAbsoluteTime(time);
                eventSequence->addEvent(event);
            }
            
            measureIndex += 1;
            if (time > measureStartTime + measureDuration)
                time = measureStartTime + measureDuration;
            measureStartTime = time;
        }
    }

    return eventSequence;
}

void EventFactory::fillWallTimes(EventSequence& eventSequence) {
    double tempo = 60.0;
    int divisions = 1;
    dom::time_t time = _startTime;
    double wallTime = 0.0;

    for (auto& event : eventSequence.events()) {
        divisions = _scoreProperties.divisions(event.measureIndex());
        tempo = _scoreProperties.tempo(event.measureIndex(), event.measureTime());

        const auto divisionsPerBeat = _scoreProperties.divisionsPerBeat(event.measureIndex());
        const auto divisionDuration = 60.0 / (divisionsPerBeat * tempo); // In seconds

        wallTime += divisionDuration * static_cast<double>(event.absoluteTime() - time);
        time = event.absoluteTime();

        event.setWallTime(wallTime);
        event.setWallTimeDuration(event.maxDuration() * divisionDuration);
    }

}

bool EventFactory::isTieStart(const mxml::dom::Note& note) {
    if (note.notations) {
        const auto& notations = note.notations;
        for (auto& tie : notations->ties) {
            if (tie->type() == mxml::dom::kStart || tie->type() == mxml::dom::kContinue)
                return true;
        }
    }

    return false;
}

bool EventFactory::isTieStop(const mxml::dom::Note& note) {
    if (note.notations) {
        const auto& notations = note.notations;
        for (auto& tie : notations->ties) {
            if (tie->type() == mxml::dom::kStop || tie->type() == mxml::dom::kContinue)
                return true;
        }
    }
    
    return false;
}

} // namespace mxml
