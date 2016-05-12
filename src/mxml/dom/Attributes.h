// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

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

    Optional<int> staves() const {
        return _staves;
    }
    void setStaves(Optional<int> staves);
    
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
    void setClef(int number, std::unique_ptr<Clef>&& clef);
    
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
    void setKey(int number, std::unique_ptr<Key> key);
    
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
