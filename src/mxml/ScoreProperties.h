// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once
#include "attributes/AlterSequence.h"
#include "attributes/ClefSequence.h"
#include "attributes/DivisionsSequence.h"
#include "attributes/KeySequence.h"
#include "attributes/TimeSequence.h"
#include "Jump.h"
#include "Loop.h"

#include <mxml/dom/Attributes.h>
#include <mxml/dom/Direction.h>
#include <mxml/dom/Print.h>
#include <mxml/dom/Score.h>
#include <mxml/dom/Sound.h>

#include <algorithm>
#include <map>
#include <set>
#include <vector>


namespace mxml {

namespace dom {
    class Chord;
    class Note;
    class Pitch;
}

class ScoreProperties {
public:
    enum class LayoutType : int {
        Scroll,
        Page
    };

public:
    ScoreProperties(const dom::Score& score, LayoutType layoutType = LayoutType::Scroll);

    /**
     Get the number of measures in the score.
     */
    std::size_t measureCount() const { return _measureCount; }

    /**
     Get active key for the given part, measure, staff and time.
     */
    const dom::Key* key(std::size_t partIndex, std::size_t measureIndex, int staff, dom::time_t time) const {
        return _keySequence.find(partIndex, measureIndex, staff, time);
    }

    /**
     Get active clef for the given part, measure, staff and time.
     */
    const dom::Clef* clef(std::size_t partIndex, std::size_t measureIndex, int staff, dom::time_t time) const {
        return _clefSequence.find(partIndex, measureIndex, staff, time);
    }

    /**
     Get the clef for a given note.
     */
    const dom::Clef* clef(const dom::Note& note) const;

    /**
     Get active time signature for the given measure.
     */
    const dom::Time* time(std::size_t measureIndex) const {
        return _timeSequence.find(measureIndex);
    }

    /**
     Get active divisions for the given measure.
     */
    dom::time_t divisions(std::size_t measureIndex) const {
        return _divisionsSequence.find(measureIndex);
    }

    /**
     Get the number of divsions per beat for the given measure.
     */
    dom::time_t divisionsPerBeat(std::size_t measureIndex) const;

    /**
     Get the number of divsions per measure for the given measure.
     */
    dom::time_t divisionsPerMeasure(std::size_t measureIndex) const;

    /**
     Get the number of parts in the score.
     */
    std::size_t partCount() const;

    /**
     Get the number of staves for a given part. This method assumes that the staves value gets set once and never
     changes.
     */
    int staves(std::size_t partIndex) const;

    /**
     Get the total number of staves for all parts. This method assumes that the staves value gets set once and never
     changes.
     */
    int staves() const;

    /**
     Get the chromatic alteration for a particular note, not including alterations at the same time as the note.
     */
    int alter(const dom::Note& note) const;

    /**
     Get the tempo at the given measure and time.
     */
    float tempo(std::size_t measureIndex, dom::time_t time) const;

    /**
     Get the dynamics for the given note.
     */
    float dynamics(const dom::Note& note) const;

    const std::vector<Loop>& loops() const { return _loops; }
    const std::vector<Jump>& jumps() const { return _jumps; }

    /**
     Get the loop that contains the given measure, if any.
     */
    const Loop* loop(std::size_t measureIndex) const;

    /**
     Get the jumps from the given measure, if any.
     */
    std::vector<Jump> jumps(std::size_t measureIndex) const;

    /**
     Get the size of the octave shift for the given part, measure, staff and time.
     */
    int octaveShift(std::size_t partIndex, std::size_t measureIndex, int staff, dom::time_t time) const;

    /**
     Get the system index for the given measure index.
     */
    std::size_t systemIndex(std::size_t measureIndex) const;

    /**
     Get the range of measures for the given system index.
     */
    std::pair<std::size_t, std::size_t> measureRange(std::size_t systemIndex) const;

    std::size_t systemCount() const {
        return _systemBegins.size();
    }

    /**
     Get the page index for the given measure index.
     */
    std::size_t pageIndex(std::size_t measureIndex) const;

    std::size_t pageCount() const {
        return _pageBegins.size();
    }

    /**
     Get the layout type for the score.
     */
    LayoutType layoutType() const {
        return _layoutType;
    }

    
private:
    /**
     Get the dynamics at the given part, measure, staff and time.
     */
    float dynamics(std::size_t partIndex, std::size_t measureIndex, int staff, dom::time_t time) const;

protected:
    struct SoundRef {
        std::size_t partIndex;
        std::size_t measureIndex;
        dom::time_t time;
        dom::Optional<int> staff;
        const dom::Sound* sound;

        bool operator==(const SoundRef& rhs) const {
            return partIndex == rhs.partIndex && measureIndex == rhs.measureIndex && time == rhs.time && sound == rhs.sound;
        }
        bool operator<(const SoundRef& rhs) const {
            if (measureIndex < rhs.measureIndex)
                return true;
            if (measureIndex > rhs.measureIndex)
                return false;

            if (time < rhs.time)
                return true;
            if (time > rhs.time)
                return false;

            if (partIndex < rhs.partIndex)
                return true;
            if (partIndex > rhs.partIndex)
                return false;

            return sound < rhs.sound;
        }

    };

    struct DirectionRef {
        std::size_t partIndex;
        std::size_t measureIndex;
        dom::time_t time;
        dom::Optional<int> staff;
        const dom::Direction* direction;

        bool operator==(const DirectionRef& rhs) const {
            return partIndex == rhs.partIndex && measureIndex == rhs.measureIndex && time == rhs.time && staff == rhs.staff && direction == rhs.direction;
        }
        bool operator<(const DirectionRef& rhs) const {
            if (measureIndex < rhs.measureIndex)
                return true;
            if (measureIndex > rhs.measureIndex)
                return false;

            if (time < rhs.time)
                return true;
            if (time > rhs.time)
                return false;

            if (partIndex < rhs.partIndex)
                return true;
            if (partIndex > rhs.partIndex)
                return false;

            if (staff.value() < rhs.staff.value())
                return true;
            if (staff.value() > rhs.staff.value())
                return false;

            return direction < rhs.direction;
        }
        
    };

protected:
    void process(std::size_t partIndex, const dom::Measure& measure);
    void process(std::size_t partIndex, std::size_t measureIndex, const dom::Attributes& attributes);
    void process(std::size_t partIndex, std::size_t measureIndex, const dom::Direction& direction);
    void process(std::size_t partIndex, std::size_t measureIndex, const dom::Print& print);
    void process(std::size_t partIndex, std::size_t measureIndex, const dom::Chord& chord);

private:
    std::set<DirectionRef> _directions;
    std::set<SoundRef> _sounds;

    ClefSequence _clefSequence;
    KeySequence _keySequence;
    TimeSequence _timeSequence;
    DivisionsSequence _divisionsSequence;
    AlterSequence _alterSequence;

    std::vector<Loop> _loops;
    std::vector<Jump> _jumps;
    std::vector<int> _staves;
    std::size_t _measureCount;

    std::set<std::size_t> _systemBeginsSet;
    std::vector<std::size_t> _systemBegins;
    std::set<std::size_t> _pageBeginsSet;
    std::vector<std::size_t> _pageBegins;

    const LayoutType _layoutType;
};
    
}
