// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "DivisionsSequence.h"

#include <algorithm>
#include <iterator>


namespace mxml {

void DivisionsSequence::addFromAttributes(std::size_t measureIndex, const dom::Attributes& attributes) {
    if (!attributes.divisions().isPresent())
        return;

    Item item;
    item.index.time.measureIndex = measureIndex;
    item.value = attributes.divisions();
    _items.push_back(item);
}

int DivisionsSequence::find(std::size_t measureIndex) const {
    Item item;
    item.index.time.measureIndex = measureIndex;

    auto end = std::upper_bound(_items.begin(), _items.end(), item);
    auto it = std::reverse_iterator<std::vector<Item>::const_iterator>(end);
    if (it == _items.rend())
        return 1;

    return it->value;
}
    
} // namespace mxml
