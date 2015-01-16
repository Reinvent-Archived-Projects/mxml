//  Created by Alejandro Isaza on 2014-05-15.
//  Copyright (c) 2014 Venture Media Labs. All rights reserved.

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
  _time(0)
{}

std::unique_ptr<EventSequence> EventFactory::build() {
    for (auto& part : _score.parts()) {
        _part = part.get();
        _measureStartTime = 0;
        _time = 0;
        for (std::size_t measureIndex = 0; measureIndex < part->measures().size(); measureIndex += 1) {
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

    auto nextTime = _measureStartTime + Attributes::divisionsPerMeasure(_scoreProperties.divisions(measure.index()), *_scoreProperties.time(measure.index()));
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
    onEvent.addOnNote(note);

    auto& offEvent = event(measureIndex, note.start() + note.duration(), _time + note.duration());
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
    for (std::size_t measureIndex = 0; measureIndex < _scoreProperties.measureCount(); measureIndex += 1) {
        auto divisions = _scoreProperties.divisions(measureIndex);
        auto time = _scoreProperties.time(measureIndex);
        auto divisionsPerBeat = Attributes::divisionsPerBeat(divisions, *time);
        auto divisionsPerMeasure = divisionsPerBeat * time->beats();

        for (dom::time_t time = 0; time < divisionsPerMeasure; time += divisionsPerBeat) {
            auto& e = event(measureIndex, time, absoluteTime);
            e.setBeatMark(true);
            absoluteTime += divisionsPerBeat;
        }
    }
}

std::unique_ptr<EventSequence> EventFactory::unroll() {
    auto eventSequence = std::unique_ptr<EventSequence>(new EventSequence(_scoreProperties));

    std::size_t measureIndex = 0;
    dom::time_t measureStartTime = 0;
    dom::time_t time = 0;

    std::map<Loop, std::size_t> loopCounts;
    std::map<Jump, std::size_t> jumpCounts;

    while (measureIndex < _scoreProperties.measureCount()) {
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
            auto& count = jumpCounts[jump];
            if (jump.forward() && count == 1) {
                measureIndex = jump.to;
                skipped = true;
            } else if (!jump.forward() && count == 0) {
                measureIndex = jump.to;
                skipped = true;
            }
            count += 1;
        }

        if (!skipped) {
            auto measureDuration = Attributes::divisionsPerMeasure(_scoreProperties.divisions(measureIndex), *_scoreProperties.time(measureIndex));
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
    dom::time_t time = 0;
    double wallTime = 0.0;

    std::size_t mi = 0;
    for (auto& event : eventSequence.events()) {
        if (event.measureIndex() < mi)
            mi = mi;
        mi =event.measureIndex();
        divisions = _scoreProperties.divisions(event.measureIndex());
        tempo = _scoreProperties.tempo(event.measureIndex(), event.measureTime());

        double divisionDuration = 60.0 / (divisions * tempo);
        wallTime += divisionDuration * static_cast<double>(event.absoluteTime() - time);
        time = event.absoluteTime();

        event.setWallTime(wallTime);
        event.setWallTimeDuration(event.maxDuration() * divisionDuration);
    }

}

} // namespace mxml
