//  Created by Alejandro Isaza on 2014-05-15.
//  Copyright (c) 2014 Venture Media Labs. All rights reserved.

#include "Event.h"

namespace mxml {

Event::Event(const dom::Score& score)
: _score(&score), _measureIndex(), _time()
{
}

Event::Event(const dom::Score& score, std::size_t measureIndex, int time)
: _score(&score), _measureIndex(measureIndex), _time(time)
{
}

dom::time_t Event::maxDuration() const {
    auto it = std::max_element(_onNotes.begin(), _onNotes.end(), [](const dom::Note* n1, const dom::Note* n2) {
        return n1->duration() < n2->duration();
    });
    if (it == _onNotes.end())
        return 0;
    return (*it)->duration();
}

} // namespace mxml
