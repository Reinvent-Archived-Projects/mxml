//  Created by Steve Hosking on 2014-12-18.
//  Copyright (c) 2014 Venture Media Labs. All rights reserved.

#pragma once
#include <mxml/dom/Attributes.h>
#include <mxml/dom/Measure.h>

#include <algorithm>
#include <map>
#include <vector>


namespace mxml {
    
class AttributesManager {
public:
    struct Alter {
        std::size_t measureIndex;
        dom::time_t time;

        int staff;
        int octave;
        dom::Pitch::Step step;

        int amount;

        Alter() : measureIndex(), time(0), staff(1), octave(4), step(), amount() {}
    };

public:
    AttributesManager();
    
    void addAllAttributes(const dom::Measure& measure);
    void addAttributes(const dom::Attributes& attributes);
    void addAlter(const dom::Note& note);
    void addAlter(std::size_t measureIndex, dom::time_t time, int staff, int octave, dom::Pitch::Step step, int alterAmount);

    /**
     Get active key for the given measure, staff and time.
     */
    const dom::Key* key(std::size_t measureIndex, int staff, time_t time) const;

    /**
     Get active clef for the given measure, staff and time.
     */
    const dom::Clef* clef(std::size_t measureIndex, int staff, time_t time) const;

    /**
     Get the clef for a given note.
     */
    const dom::Clef* clef(const dom::Note& note) const;

    /**
     Get active divisions for the given measure.
     */
    int divisions(std::size_t measureIndex) const;

    /**
     Get the number of staves. This method assumes that the staves value gets set once and never changes.
     */
    int staves() const;

    /**
     Get the current number of the divistion for the last measure that was processed.
     */
    int divisions() const;

    /**
     Get the current time signature for the last measure that was processed.
     */
    const dom::Time* time() const;

    /**
     Get the chromatic alteration for a particular note.
     */
    int alter(const dom::Note& note) const;
    int alter(std::size_t measureIndex, dom::time_t time, int staff, int octave, dom::Pitch::Step step) const;

protected:
    /**
     Get an attribute value for the given measure and time.

     @param measure      The measure
     @param time         The time relative to the start of the measure
     @param defaultValue The default value to return if no attribute has the requested value
     @param cooser       The function that captures a value from dom::Attributes or returns its second argument if the
                         value is not present.

     @return The most recent attribute value in the measure at the given time or the default value.
     */
    template <typename T>
    T getAttribute(std::size_t measureIndex, time_t time, T defaultValue, std::function<T (const dom::Attributes&, T)> chooser) const {
        T current = defaultValue;
        for (auto attributes : _attributes) {
            auto measure = static_cast<const dom::Measure*>(attributes->parent());
            if (measure->index() > measureIndex || (measure->index() == measureIndex && attributes->start() > time))
                return current;
            current = chooser(*attributes, current);
        }
        return current;
    }

private:
    std::vector<const dom::Attributes*> _attributes;
    std::vector<Alter> _alters;
    int _staves;

    static const dom::Key _defaultKey;
    static const dom::Clef _defaultClef;
    static const dom::Time _defaultTime;
};
    
}