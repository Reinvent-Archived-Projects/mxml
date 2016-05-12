// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once
#include <mxml/dom/Note.h>
#include <mxml/dom/Score.h>

namespace mxml {

struct MeasureLocation {
    std::size_t measureIndex = 0;
    dom::time_t division = 0;

    bool operator==(const MeasureLocation& rhs) const noexcept {
        return measureIndex == rhs.measureIndex && division == rhs.division;
    }
    bool operator!=(const MeasureLocation& rhs) const noexcept {
        return !operator==(rhs);
    }
    bool operator<(const MeasureLocation& rhs) const noexcept {
        return measureIndex < rhs.measureIndex || (measureIndex == rhs.measureIndex && division < rhs.division);
    }
};

class Event {
public:
    Event() = default;
    explicit Event(const dom::Score& score);
    Event(const dom::Score& score, std::size_t measureIndex, dom::time_t measureTime, dom::time_t absoluteTime);
    Event(const Event& event) = default;
    
    const dom::Score& score() const {
        return *_score;
    }
    void setScore(const dom::Score& score) {
        _score = &score;
    }

    MeasureLocation measureLocation() const {
        MeasureLocation location;
        location.measureIndex = _measureIndex;
        location.division = _measureTime;
        return location;
    }

    std::size_t measureIndex() const {
        return _measureIndex;
    }
    void setMeasureIndex(std::size_t measureIndex) {
        _measureIndex = measureIndex;
    }
    
    dom::time_t measureTime() const {
        return _measureTime;
    }
    void setMeasureTime(dom::time_t time) {
        _measureTime = time;
    }

    dom::time_t absoluteTime() const {
        return _absoluteTime;
    }
    void setAbsoluteTime(dom::time_t time) {
        _absoluteTime = time;
    }

    bool isBeatMark() const {
        return _beatMark;
    }
    void setBeatMark(bool value) {
        _beatMark = value;
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
    std::vector<const dom::Note*>& onNotes() {
        return _onNotes;
    }
    const std::vector<const dom::Note*>& offNotes() const {
        return _offNotes;
    }
    std::vector<const dom::Note*>& offNotes() {
        return _offNotes;
    }
    
    void addOnNote(const dom::Note& note) {
        _onNotes.push_back(&note);
    }
    void addOffNote(const dom::Note& note) {
        _offNotes.push_back(&note);
    }
    
    bool operator<(const Event& rhs) const {
        return _absoluteTime < rhs._absoluteTime;
    }
    
private:
    const dom::Score* _score;

    std::size_t _measureIndex;
    dom::time_t _measureTime;
    dom::time_t _absoluteTime;
    bool _beatMark;

    double _wallTime;
    double _wallTimeDuration;
    
    std::vector<const dom::Note*> _onNotes;
    std::vector<const dom::Note*> _offNotes;
};

} // namespace mxml
