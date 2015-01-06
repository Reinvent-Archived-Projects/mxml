//  Created by Steve Hosking on 2014-12-18.
//  Copyright (c) 2014 Venture Media Labs. All rights reserved.

#pragma once
#include <mxml/dom/Attributes.h>
#include <mxml/dom/Measure.h>

#include <map>

namespace mxml {
    
class AttributesManager {
public:
    void addAllAttributes(const dom::Measure& measure);
    void addAttributes(const dom::Attributes& attributes);
    
    /**
     Get active key for the given measure, staff and time.
     */
    const dom::Key* key(const dom::Measure& measure, int staff, time_t time) const;

    /**
     Get active clef for the given measure, staff and time.
     */
    const dom::Clef* clef(const dom::Measure& measure, int staff, time_t time) const;

    /**
     Get the clef for a given note.
     */
    const dom::Clef* clef(const dom::Note& note) const;

    /**
     Get active divisions for the given measure.
     */
    int divisions(const dom::Measure& measure) const;

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
    T getAttribute(const dom::Measure& measure, time_t time, T defaultValue, std::function<T (const dom::Attributes&, T)> chooser) const {
        T current = defaultValue;

        bool reachedNotesMeasure = false;

        for (auto& attribute : _attributes) {
            if (reachedNotesMeasure)
                break;

            if (attribute->parent() == &measure) {
                reachedNotesMeasure = true;

                if (attribute->start() > time)
                    break;
            }
            
            current = chooser(*attribute, current);
        }
        
        return current;
    }

private:
    std::vector<const dom::Attributes*> _attributes;
    dom::Key _defaultKey;
    dom::Clef _defaultClef;
    dom::Time _defaultTime;
    
};
    
}