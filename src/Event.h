//  Created by Alejandro Isaza on 2014-05-15.
//  Copyright (c) 2014 Venture Media Labs. All rights reserved.

#pragma once
#include "dom/Score.h"

namespace mxml {

class Event {
public:
    Event() = default;
    explicit Event(const dom::Score& score);
    Event(const dom::Score& score, std::size_t measureIndex, int time);
    Event(const Event& event) = default;
    
    const dom::Score& score() const {
        return *_score;
    }
    void setScore(const dom::Score& score) {
        _score = &score;
    }
    
    std::size_t measureIndex() const {
        return _measureIndex;
    }
    void setMeasureIndex(std::size_t measureIndex) {
        _measureIndex = measureIndex;
    }
    
    dom::time_t time() const {
        return _time;
    }
    void setTime(dom::time_t time) {
        _time = time;
    }

    double wallTime() const {
        return _wallTime;
    }
    void setWallTime(double time) {
        _wallTime = time;
    }

    double wallTimeDuration() const {
        return _wallTimeDuration;
    }
    void setWallTimeDuration(double duration) {
        _wallTimeDuration = duration;
    }

    dom::time_t maxDuration() const;

    const std::vector<const dom::Note*>& onNotes() const {
        return _onNotes;
    }
    const std::vector<const dom::Note*>& offNotes() const {
        return _offNotes;
    }
    
    void addOnNote(const dom::Note& note) {
        _onNotes.push_back(&note);
    }
    void addOffNote(const dom::Note& note) {
        _offNotes.push_back(&note);
    }
    
    bool operator<(const Event& rhs) const {
        return _time < rhs._time;
    }
    
private:
    const dom::Score* _score;

    std::size_t _measureIndex;
    dom::time_t _time;

    double _wallTime;
    double _wallTimeDuration;
    
    std::vector<const dom::Note*> _onNotes;
    std::vector<const dom::Note*> _offNotes;
};

} // namespace mxml
