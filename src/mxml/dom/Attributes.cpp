// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Attributes.h"

namespace mxml {
namespace dom {

static const int kMaximumStaves = 100;

void Attributes::setStaves(Optional<int> staves) {
    assert(staves > 0 && staves < kMaximumStaves);
    if (staves > kMaximumStaves)
        staves = kMaximumStaves;

    while (_clefs.size() < staves)
        _clefs.emplace_back();

    auto reuseKey = _keys.back().get();
    while (_keys.size() < staves) {
        if (reuseKey) {
            _keys.emplace_back(new Key(*reuseKey));
            _keys.back()->setNumber(static_cast<int>(_keys.size()));
        } else {
            _keys.emplace_back();
        }
    }

    _staves = staves;
}

void Attributes::setClef(int number, std::unique_ptr<Clef>&& clef) {
    assert(number > 0 && number < kMaximumStaves);
    while (_clefs.size() < number)
        _clefs.emplace_back();
    _clefs[number - 1] = std::move(clef);
}

void Attributes::setKey(int number, std::unique_ptr<Key> key) {
    assert(number > 0 && number < kMaximumStaves);
    while (_keys.size() < number)
        _keys.emplace_back();
    _keys[number - 1] = std::move(key);
}

}
}
