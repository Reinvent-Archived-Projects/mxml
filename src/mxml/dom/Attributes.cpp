//  Created by Steve Hosking on 2015-02-03.
//  Copyright (c) 2015 Venture Media Labs. All rights reserved.

#include "Attributes.h"

namespace mxml {
namespace dom {

static const int kMaximumStaves = 100;

void Attributes::setStaves(Optional<int> staves) {
    assert(staves > 0 && staves < kMaximumStaves);
    if (staves > kMaximumStaves)
        staves = kMaximumStaves;

    if (_staves > 0 && staves > _staves) {
        auto& lastClef = _clefs.back();
        auto& lastKey = _keys.back();

        for (int i = _staves; i < staves; i += 1) {
            if (lastClef) {
                _clefs.emplace_back(new Clef(*lastClef));
                _clefs.back()->setNumber(i);
            } else {
                _clefs.emplace_back();
            }

            if (lastKey) {
                _keys.emplace_back(new Key(*lastKey));
                _keys.back()->setNumber(i);
            } else {
                _keys.emplace_back();
            }
        }
    } else {
        _clefs.resize(staves);
        _keys.resize(staves);
    }

    _staves = staves;
}

void Attributes::setClef(int number, std::unique_ptr<Clef>&& clef) {
    assert(number > 0 && number < kMaximumStaves);
    while (_clefs.size() <= number)
        _clefs.emplace_back();
    _clefs[number - 1] = std::move(clef);
}

void Attributes::setKey(int number, std::unique_ptr<Key> key) {
    assert(number > 0 && number < kMaximumStaves);
    while (_keys.size() <= number)
        _keys.emplace_back();
    _keys[number - 1] = std::move(key);

    if (number == 1) {
        for (int i = 1; i < _staves; i += 1) {
            _keys[i] = std::move(key);
            _keys[i]->setNumber(i);
        }
    }
}

}
}
