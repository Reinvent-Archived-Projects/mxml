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
    Attributes() : _divisions(1), _staves(1), _clefs(1), _keys(1), _time(), _start() {}
    
    Optional<int> divisions() const {
        return _divisions;
    }
    void setDivisions(Optional<int> divisions) {
        _divisions = divisions;
    }

    static int divisionsPerBeat(int divisions, const Time& time) {
        return divisions * 4 / time.beatType();
    }

    static int divisionsPerMeasure(int divisions, const Time& time) {
        return divisionsPerBeat(divisions, time) * time.beats();
    }

    Optional<int> staves() const {
        return _staves;
    }
    void setStaves(Optional<int> staves) {
        if (_staves.value() > 0 && staves.value() > _staves.value()) {
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
    
    const Clef* clef(int number) const {
        if (number > 0 && number <= _clefs.size())
            return _clefs[number - 1].get();
        return nullptr;
    }
    Clef* clef(int number) {
        if (number > 0 && number <= _clefs.size())
            return _clefs[number - 1].get();
        return nullptr;
    }
    void setClef(int number, std::unique_ptr<Clef>&& clef) {
        assert(number >= 1 && number <= _clefs.size() + 1);
        if (_clefs.size() >= number)
            _clefs[number - 1] = std::move(clef);
    }
    
    const Key* key(int number) const {
        if (number > 0 && number <= _keys.size())
            return _keys[number - 1].get();
        return nullptr;
    }
    Key* key(int number) {
        if (number > 0 && number <= _keys.size())
            return _keys[number - 1].get();
        return nullptr;
    }
    void setKey(int number, std::unique_ptr<Key> key) {
        assert(number >= 1 && number <= _keys.size() + 1);
        if (_keys.size() >= number)
            _keys[number - 1] = std::move(key);
        
        if (number == 1) {
            for (int i = 1; i < _staves; i += 1) {
                _keys[i] = std::move(key);
                _keys[i]->setNumber(i);
            }
        }
    }
    
    const Time* time() const {
        return _time.get();
    }
    Time* time() {
        return _time.get();
    }
    void setTime(std::unique_ptr<Time> time) {
        _time = std::move(time);
    }
    
    void setStart(time_t start) {
        _start = start;
    }
    time_t start() const {
        return _start;
    }
    
private:
    Optional<int> _divisions;
    Optional<int> _staves;
    std::vector<std::unique_ptr<Clef>> _clefs;
    std::vector<std::unique_ptr<Key>> _keys;
    std::unique_ptr<Time> _time;
    time_t _start;
};

} // namespace dom
} // namespace mxml
