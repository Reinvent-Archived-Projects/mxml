// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Event.h"

namespace mxml {

Event::Event(const dom::Score& score)
: _score(&score),
  _measureIndex(),
  _measureTime(),
  _absoluteTime(),
  _beatMark()
{}

Event::Event(const dom::Score& score, std::size_t measureIndex, dom::time_t measureTime, dom::time_t absoluteTime)
: _score(&score),
  _measureIndex(measureIndex),
  _measureTime(measureTime),
  _absoluteTime(absoluteTime),
  _beatMark()
{}

dom::time_t Event::maxDuration() const {
    auto it = std::max_element(_onNotes.begin(), _onNotes.end(), [](const dom::Note* n1, const dom::Note* n2) {
        return n1->duration() < n2->duration();
    });
    if (it == _onNotes.end())
        return 0;
    return (*it)->duration();
}

} // namespace mxml
