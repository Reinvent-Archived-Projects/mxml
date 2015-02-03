//  Created by Steve Hosking on 2014-12-18.
//  Copyright (c) 2014 Venture Media Labs. All rights reserved.

#pragma once
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
    ScoreProperties(const dom::Score& score);

    /**
     Get the number of measures in the score.
     */
    std::size_t measureCount() const { return _measureCount; }

    /**
     Get active key for the given part, measure, staff and time.
     */
    const dom::Key* key(std::size_t partIndex, std::size_t measureIndex, int staff, time_t time) const;

    /**
     Get active clef for the given part, measure, staff and time.
     */
    const dom::Clef* clef(std::size_t partIndex, std::size_t measureIndex, int staff, time_t time) const;

    /**
     Get the clef for a given note.
     */
    const dom::Clef* clef(const dom::Note& note) const;

    /**
     Get active time signature for the given measure.
     */
    const dom::Time* time(std::size_t measureIndex) const;

    /**
     Get active divisions for the given measure.
     */
    dom::time_t divisions(std::size_t measureIndex) const;

    /**
     Get the number of divsions per beat for the given measure.
     */
    dom::time_t divisionsPerBeat(std::size_t measureIndex) const;

    /**
     Get the number of divsions per measure for the given measure.
     */
    dom::time_t divisionsPerMeasure(std::size_t measureIndex) const;

    /**
     Get the number of staves for a given part. This method assumes that the staves value gets set once and never
     changes.
     */
    int staves(std::size_t partIndex) const;

    /**
     Get the chromatic alteration for a particular note.
     */
    int alter(const dom::Note& note) const;

    /**
     Get the previous chromatic alteration for a particular note.
     */
    int previousAlter(const dom::Note& note) const;

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
    
private:
    /**
     Get the chromatic alteration for a given part, measure, time, staff, octave and step.
     */
    int alter(std::size_t partIndex, std::size_t measureIndex, int staff, dom::time_t time, int octave, dom::Pitch::Step step) const;
    
    /**
     Get the dynamics at the given part, measure, staff and time.
     */
    float dynamics(std::size_t partIndex, std::size_t measureIndex, int staff, dom::time_t time) const;

protected:
    struct AttributesRef {
        std::size_t partIndex;
        std::size_t measureIndex;
        dom::time_t time;
        const dom::Attributes* attributes;

        bool operator==(const AttributesRef& rhs) const {
            return partIndex == rhs.partIndex && measureIndex == rhs.measureIndex && time == rhs.time && attributes == rhs.attributes;
        }
        bool operator<(const AttributesRef& rhs) const {
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

            return attributes < rhs.attributes;
        }
    };

    struct PitchRef {
        std::size_t partIndex;
        std::size_t measureIndex;
        dom::time_t time;
        int staff;
        const dom::Pitch* pitch;

        bool operator==(const PitchRef& rhs) const {
            return partIndex == rhs.partIndex && measureIndex == rhs.measureIndex && time == rhs.time && staff == rhs.staff && pitch == rhs.pitch;
        }
        bool operator<(const PitchRef& rhs) const {
            if (measureIndex < rhs.measureIndex)
                return true;
            if (measureIndex > rhs.measureIndex)
                return false;

            if (time < rhs.time)
                return true;
            if (time > rhs.time)
                return false;

            if (staff < rhs.staff)
                return true;
            if (staff > rhs.staff)
                return false;

            if (partIndex < rhs.partIndex)
                return true;
            if (partIndex > rhs.partIndex)
                return false;

            return pitch < rhs.pitch;
        }
    };

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
    void process(std::size_t partIndex, std::size_t measureIndex, const dom::Note& note);

    /**
     Get an attribute value for the given part, measure and time.

     @param partIndex    The part index
     @param measureIndex The measure index
     @param time         The time relative to the start of the measure
     @param defaultValue The default value to return if no attribute has the requested value
     @param cooser       The function that captures a value from dom::Attributes or returns its second argument if the
                         value is not present.

     @return The most recent attribute value in the measure at the given time or the default value.
     */
    template <typename T>
    T getAttribute(std::size_t partIndex, std::size_t measureIndex, time_t time, T defaultValue, std::function<T (const dom::Attributes&, T)> chooser) const {
        T current = defaultValue;
        for (auto& ref : _attributes) {
            if (ref.partIndex != partIndex)
                continue;

            if (ref.measureIndex > measureIndex || (ref.measureIndex == measureIndex && ref.time > time))
                return current;
            current = chooser(*ref.attributes, current);
        }
        return current;
    }

    /**
     Get an attribute value for the given measure and time.

     @param measureIndex The measure index
     @param time         The time relative to the start of the measure
     @param defaultValue The default value to return if no attribute has the requested value
     @param cooser       The function that captures a value from dom::Attributes or returns its second argument if the
     value is not present.

     @return The most recent attribute value in the measure at the given time or the default value.
     */
    template <typename T>
    T getAttribute(std::size_t measureIndex, time_t time, T defaultValue, std::function<T (const dom::Attributes&, T)> chooser) const {
        T current = defaultValue;
        for (auto& ref : _attributes) {
            if (ref.measureIndex > measureIndex || (ref.measureIndex == measureIndex && ref.time > time))
                return current;
            current = chooser(*ref.attributes, current);
        }
        return current;
    }

    /**
     Get an attribute value for the given measure.

     @param measureIndex The measure index
     @param defaultValue The default value to return if no attribute has the requested value
     @param cooser       The function that captures a value from dom::Attributes or returns its second argument if the
     value is not present.

     @return The most recent attribute value in the measure at the given time or the default value.
     */
    template <typename T>
    T getAttribute(std::size_t measureIndex, T defaultValue, std::function<T (const dom::Attributes&, T)> chooser) const {
        T current = defaultValue;
        for (auto& ref : _attributes) {
            if (ref.measureIndex > measureIndex)
                return current;
            current = chooser(*ref.attributes, current);
        }
        return current;
    }

private:
    std::set<AttributesRef> _attributes;
    std::set<DirectionRef> _directions;
    std::set<PitchRef> _pitches;
    std::set<SoundRef> _sounds;
    std::vector<Loop> _loops;
    std::vector<Jump> _jumps;
    std::vector<int> _staves;
    std::size_t _measureCount;

    std::set<std::size_t> _systemBeginsSet;
    std::vector<std::size_t> _systemBegins;

    static const dom::Time _defaultTime;
};
    
}