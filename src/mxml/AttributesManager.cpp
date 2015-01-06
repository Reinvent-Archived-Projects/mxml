//  Created by Steve Hosking on 2014-12-18.
//  Copyright (c) 2014 Venture Media Labs. All rights reserved.

#include "AttributesManager.h"

namespace mxml {

AttributesManager::AttributesManager() : _attributes(), _staves(0) {}

void AttributesManager::addAllAttributes(const dom::Measure& measure) {
    for (auto& node : measure.nodes()) {
        auto attributes = dynamic_cast<const dom::Attributes*>(node.get());
        if (attributes)
            addAttributes(*attributes);
    }
}

void AttributesManager::addAttributes(const dom::Attributes& attributes) {
    _attributes.push_back(&attributes);

    if (attributes.staves().isPresent()) {
        assert(_staves == 0); // We don't support changing the number of staves mid-song
        _staves = attributes.staves();
    }
}

const dom::Key* AttributesManager::key(std::size_t measureIndex, int staff, time_t time) const {
    return getAttribute<const dom::Key*>(measureIndex, time, &_defaultKey, [&](const dom::Attributes& attribute, const dom::Key* previous) {
        if (attribute.key(staff))
            return attribute.key(staff);
        else if (attribute.key(1))
            return attribute.key(1);
        return previous;
    });
}

const dom::Clef* AttributesManager::clef(std::size_t measureIndex, int staff, time_t time) const {
    return getAttribute<const dom::Clef*>(measureIndex, time, &_defaultClef, [&](const dom::Attributes& attribute, const dom::Clef* previous) {
        if (attribute.clef(staff))
            return attribute.clef(staff);
        else if (attribute.clef(1))
            return attribute.clef(1);
        return previous;
    });
}

int AttributesManager::divisions(std::size_t measureIndex) const {
    return getAttribute<int>(measureIndex, 0, 1, [&](const dom::Attributes& attribute, int previous) {
        if (attribute.divisions().isPresent())
            return attribute.divisions().value();
        return previous;
    });
}

const dom::Clef* AttributesManager::clef(const dom::Note& note) const {
    return clef(note.measure()->index(), note.staff(), note.start());
}

int AttributesManager::staves() const {
    return _staves;
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
