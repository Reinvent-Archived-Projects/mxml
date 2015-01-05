//  Created by Steve Hosking on 2014-12-18.
//  Copyright (c) 2014 Venture Media Labs. All rights reserved.

#pragma once
#include <mxml/dom/Attributes.h>

#include <map>

namespace mxml {
    
class AttributesManager {
public:
    void addAllAttribute(const dom::Measure& measure);
    void addAttributes(const dom::Attributes& attributes);
    
    /**
     Get active key
     @param measure
     @param staff
     @param time time relative to the measure param
     */
    const dom::Key* key(const dom::Measure& measure, int staff, time_t time) const;
    const dom::Clef* clef(const dom::Measure& measure, int staff, time_t time) const;
    
    const dom::Clef* clef(const dom::Note& note) const;
    int staves() const;
    
    int divisions() const;
    const dom::Time* time() const;
    
private:
    std::vector<const dom::Attributes*> _attributes;
    dom::Key _defaultKey;
    dom::Clef _defaultClef;
    dom::Time _defaultTime;
    
};
    
}