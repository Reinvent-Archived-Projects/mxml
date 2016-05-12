// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "ClefSequence.h"

#include <algorithm>
#include <iterator>


namespace mxml {

void ClefSequence::addFromAttributes(std::size_t partIndex, std::size_t measureIndex, const dom::Attributes& attributes) {
    while (_staves.size() <= partIndex)
        _staves.push_back(1);
    if (attributes.staves().isPresent())
        _staves[partIndex] = attributes.staves();

    auto const staves = _staves[partIndex];
    for (int staff = 1; staff <= staves; staff += 1) {
        auto clef = attributes.clef(staff);
        if (!clef)
            continue;

        Item item;
        item.index.time.measureIndex = measureIndex;
        item.index.time.time = attributes.start();
        item.index.line.partIndex = partIndex;
        item.index.line.staff = staff;
        item.value = clef;
        _items.push_back(item);
    }
}

const dom::Clef* ClefSequence::find(std::size_t partIndex, std::size_t measureIndex, int staff, dom::time_t time) const {
    Item modelItem;
    modelItem.index.time.measureIndex = measureIndex;
    modelItem.index.time.time = time;
    modelItem.index.line.partIndex = partIndex;
    modelItem.index.line.staff = staff;

    auto end = std::upper_bound(_items.begin(), _items.end(), modelItem);
    auto rbegin = std::reverse_iterator<std::vector<Item>::const_iterator>(end);
    auto it = std::find_if(rbegin, _items.rend(), [=](const Item& item) {
        return item.index.line == modelItem.index.line;
    });
    if (it == _items.rend()) {
        if (staff == 1)
            return nullptr;
        else
            return find(partIndex, measureIndex, 1, time);
    }

    return it->value;
}

} // namespace mxml
