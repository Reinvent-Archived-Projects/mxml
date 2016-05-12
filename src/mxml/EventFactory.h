// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once
#include "EventSequence.h"
#include "ScoreProperties.h"

#include <map>
#include <memory>
#include <vector>


namespace mxml {

namespace dom {
    class Barline;
    class Chord;
    class Direction;
    class Measure;
    class TimedNode;
}

class EventFactory {
public:
    explicit EventFactory(const dom::Score& score, const ScoreProperties& scoreProperties);

    std::unique_ptr<EventSequence> build();
    std::unique_ptr<EventSequence> build(dom::time_t startTime, std::size_t startMeasureIndex, std::size_t endMeasureIndex);
    
private:
    void processMeasure(const dom::Measure& measure);
    void processBarline(const dom::Barline& node);
    void processTimedNode(const dom::TimedNode& node);
    void processChord(const dom::Chord& chord);
    void addNote(const dom::Note& note);

    /**
     Return the event in the current measure for the given measure time and absolute time, creates a new event if
     necessary.
     */
    Event& event(std::size_t measureIndex, dom::time_t measureTime, dom::time_t absoluteTime);

    /**
     Fill in beat mark events for every measure according to the time signature.
     */
    void setBeatMarks();

    /**
     Unroll all loops and jumps to create a linear event sequence.
     */
    std::unique_ptr<EventSequence> unroll();

    /**
     Fill the event wall times in seconds from the tempo values.
     */
    void fillWallTimes(EventSequence& eventSequence);

    bool isTieStart(const mxml::dom::Note& note);
    bool isTieStop(const mxml::dom::Note& note);

private:
    std::size_t _startMeasureIndex;
    std::size_t _endMeasureIndex;

    const dom::Score& _score;
    const ScoreProperties& _scoreProperties;
    const dom::Part* _part;

    dom::time_t _startTime;
    dom::time_t _measureStartTime;
    dom::time_t _time;

    using MapKeyType = std::pair<std::size_t, dom::time_t>;
    std::map<MapKeyType, Event> _events;
};

} // namespace mxml
