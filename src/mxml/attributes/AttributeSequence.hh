//  Created by Alejandro Isaza on 2015-02-13.
//  Copyright (c) 2015 Venture Media Labs. All rights reserved.

#include "AttributeSequence.h"

#include <mxml/dom/Measure.h>
#include <mxml/dom/Part.h>


namespace mxml {

template <typename T>
void AttributeSequence<T>::sort() {
    std::sort(_items.begin(), _items.end());
}

template <typename T>
typename AttributeSequence<T>::Index AttributeSequence<T>::indexFromNote(const dom::Note& note) {
    Index index;

    index.time.measureIndex = note.measure()->index();
    index.time.time = note.start();

    index.line.partIndex = note.measure()->part()->index();
    index.line.staff = note.staff();

    if (note.pitch()) {
        const auto& pitch = *note.pitch();
        index.line.octave = pitch.octave();
        index.line.step = pitch.step();
    }
    
    return index;
}

template <typename T>
bool AttributeSequence<T>::LineIndex::operator==(const LineIndex& rhs) const {
    return
        partIndex == rhs.partIndex &&
        staff == rhs.staff &&
        octave == rhs.octave &&
        step == rhs.step;
}

template <typename T>
bool AttributeSequence<T>::TimeIndex::operator<(const TimeIndex& rhs) const {
    if (measureIndex < rhs.measureIndex)
        return true;
    if (measureIndex > rhs.measureIndex)
        return false;
    
    return time < rhs.time;
}

} // namespace mxml
