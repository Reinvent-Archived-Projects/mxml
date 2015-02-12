//  Created by Alejandro Isaza on 2015-02-12.
//  Copyright (c) 2015 Venture Media Labs. All rights reserved.

#include "TimeSequence.h"

#include <algorithm>
#include <iterator>


namespace mxml {

const dom::Time TimeSequence::_defaultTime;

void TimeSequence::addFromAttributes(std::size_t measureIndex, const dom::Attributes& attributes) {
   if (!attributes.time())
       return;

    Item item;
    item.index.time.measureIndex = measureIndex;
    item.value = attributes.time();
    _items.push_back(item);
}

const dom::Time* TimeSequence::find(std::size_t measureIndex) const {
    Item item;
    item.index.time.measureIndex = measureIndex;

    auto end = std::upper_bound(_items.begin(), _items.end(), item);
    auto it = std::reverse_iterator<std::vector<Item>::const_iterator>(end);
    if (it == _items.rend())
        return &_defaultTime;

    return it->value;
}

} // namespace mxml
