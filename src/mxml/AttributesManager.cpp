//  Created by Steve Hosking on 2014-12-18.
//  Copyright (c) 2014 Venture Media Labs. All rights reserved.

#include "AttributesManager.h"

namespace mxml {

void AttributesManager::addAllAttributes(const dom::Measure& measure) {
    for (auto& node : measure.nodes()) {
        auto attributes = dynamic_cast<const dom::Attributes*>(node.get());
        if (attributes)
            addAttributes(*attributes);
    }
}

void AttributesManager::addAttributes(const dom::Attributes& attributes) {
    _attributes.push_back(&attributes);
}

const dom::Key* AttributesManager::key(const dom::Measure& measure, int staff, time_t time) const {
    return getAttribute<const dom::Key*>(measure, time, &_defaultKey, [&](const dom::Attributes& attribute, const dom::Key* previous) {
        if (attribute.key(staff))
            return attribute.key(staff);
        else if (attribute.key(1))
            return attribute.key(1);
        return previous;
    });
}

const dom::Clef* AttributesManager::clef(const dom::Measure& measure, int staff, time_t time) const {
    return getAttribute<const dom::Clef*>(measure, time, &_defaultClef, [&](const dom::Attributes& attribute, const dom::Clef* previous) {
        if (attribute.clef(staff))
            return attribute.clef(staff);
        else if (attribute.clef(1))
            return attribute.clef(1);
        return previous;
    });
}

int AttributesManager::divisions(const dom::Measure& measure) const {
    return getAttribute<int>(measure, 0, 1, [&](const dom::Attributes& attribute, int previous) {
        if (attribute.divisions().isPresent())
            return attribute.divisions().value();
        return previous;
    });
}

const dom::Clef* AttributesManager::clef(const dom::Note& note) const {
    return clef(*note.measure(), note.staff(), note.start());
}

int AttributesManager::staves() const {
    for (auto attribute : _attributes) {
        if (attribute->staves().isPresent())
            return attribute->staves();
    }
    
    return 0;
}

int AttributesManager::divisions() const {
    int divisions = 1;
    
    for (auto& attribute : _attributes) {
        if (attribute->divisions().isPresent())
            divisions = attribute->divisions();
    }
    
    return divisions;
}

const dom::Time* AttributesManager::time() const {
    const dom::Time* time = &_defaultTime;
    
    for (auto& attribute : _attributes) {
        if (attribute->time())
            time = attribute->time();
    }
    
    return time;
}
    
}
