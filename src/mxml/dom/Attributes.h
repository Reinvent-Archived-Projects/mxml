//  Created by Alejandro Isaza on 2/19/2014.
//  Copyright (c) 2014 Venture Media Labs Inc. All rights reserved.

#pragma once
#include "Clef.h"
#include "Key.h"
#include "Node.h"
#include "Optional.h"
#include "Time.h"

#include <cassert>
#include <vector>

namespace mxml {
namespace dom {

class Attributes : public Node {
public:
    Attributes() : _divisions(2), _staves(1), _clefs(1), _keys(1), _time() {}
    
    /**
     Set all values as absent.
     */
    void reset() {
        _divisions.setPresent(false);
        _staves.setPresent(false);
        for (int i = 0; i < _staves; i += 1) {
            _clefs[i].setPresent(false);
            _keys[i].setPresent(false);
        }
        _time.setPresent(false);
    }
    
    /**
     Copy all values from another instance and set all vaules as absent.
     */
    void reset(const Attributes& attrs) {
        _divisions.setAbsentValue(attrs.divisions());
        _staves.setAbsentValue(attrs.staves());
        _clefs.resize(_staves);
        _keys.resize(_staves);
        for (int i = 0; i < _staves; i += 1) {
            _clefs[i].setAbsentValue(attrs.clef(i + 1));
            _keys[i].setAbsentValue(attrs.key(i + 1));
        }
        _time.setAbsentValue(attrs.time());
    }
    
    Optional<int> divisions() const {
        return _divisions;
    }
    void setDivisions(Optional<int> divisions) {
        _divisions = divisions;
    }
    
    Optional<int> staves() const {
        return _staves;
    }
    void setStaves(Optional<int> staves) {
        if (_staves.value() > 0 && staves.value() > _staves.value()) {
            const Optional<Clef>& lastClef = _clefs.back();
            const Optional<Key>& lastKey = _keys.back();
            for (int i = _staves; i < staves; i += 1) {
                _clefs.push_back(lastClef);
                _clefs.back().value().setNumber(i);
                _keys.push_back(lastKey);
                _keys.back().value().setNumber(i);
            }
        } else {
            _clefs.resize(staves);
            _keys.resize(staves);
        }
        
        _staves = staves;
    }
    
    const Optional<Clef>& clef(int number) const {
        assert(number >= 1 && number <= _clefs.size());
        return _clefs[number - 1];
    }
    void setClef(int number, const Optional<Clef>& clef) {
        assert(number >= 1 && number <= _clefs.size() + 1);
        if (_clefs.size() >= number)
            _clefs[number - 1] = clef;
    }
    
    const Optional<Key>& key(int number) const {
        assert(number >= 1 && number <= _keys.size());
        return _keys[number - 1];
    }
    void setKey(int number, const Optional<Key>& key) {
        assert(number >= 1 && number <= _keys.size() + 1);
        if (_keys.size() >= number)
            _keys[number - 1] = key;
        
        if (number == 1) {
            for (int i = 1; i < _staves; i += 1) {
                _keys[i] = key;
                _keys[i].value().setNumber(i);
            }
        }
    }
    
    const Optional<Time>& time() const {
        return _time;
    }
    void setTime(const Optional<Time>& time) {
        _time = time;
    }
    void setTime(Optional<Time>&& time) {
        _time = time;
    }
    
private:
    Optional<int> _divisions;
    Optional<int> _staves;
    std::vector<Optional<Clef>> _clefs;
    std::vector<Optional<Key>> _keys;
    Optional<Time> _time;
};

} // namespace dom
} // namespace mxml
