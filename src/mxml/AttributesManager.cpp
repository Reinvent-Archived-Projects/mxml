//  Created by Steve Hosking on 2014-12-18.
//  Copyright (c) 2014 Venture Media Labs. All rights reserved.

#include "AttributesManager.h"
#include <mxml/dom/Measure.h>

namespace mxml {

void AttributesManager::addAllAttribute(const dom::Measure& measure) {
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
    const dom::Key* key = &_defaultKey;
    
    bool reachedNotesMeasure = false;
    
    for (auto& attribute : _attributes) {
        if (reachedNotesMeasure)
            break;
        
        if (attribute->parent() == &measure) {
            reachedNotesMeasure = true;
            
            if (attribute->start() > time)
                break;
        }
        
        if (attribute->key(staff))
            key = attribute->key(staff);
        else if (attribute->key(1))
            key = attribute->key(1);
    }
    
    return key;
}

const dom::Clef* AttributesManager::clef(const dom::Measure& measure, int staff, time_t time) const {
    const dom::Clef* clef = &_defaultClef;
    
    bool reachedNotesMeasure = false;
    
    for (auto& attribute : _attributes) {
        if (reachedNotesMeasure)
            break;
        
        if (attribute->parent() == &measure) {
            reachedNotesMeasure = true;
            
            if (attribute->start() > time)
                break;
        }
        
        if (attribute->clef(staff))
            clef = attribute->clef(staff);
        else if (attribute->clef(1))
            clef = attribute->clef(1);
    }
    
    return clef;
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
