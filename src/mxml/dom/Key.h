// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once
#include "Node.h"
#include "Optional.h"
#include "Pitch.h"

namespace mxml {
namespace dom {

class Key : public Node {
public:
    enum class Mode {
        Major,
        Minor,
        Dorian,
        Phrygian,
        Lydian,
        Mixolydian,
        Aeolian,
        Ionian,
        Locrian,
        None
    };
    
public:
    Key() : _number(1), _printObject(true), _cancel(), _fifths(), _mode(Mode::Major) {}
    Key(const Key& rhs) : _number(rhs.number()), _printObject(rhs.printObject()), _cancel(rhs.cancel()), _fifths(rhs.fifths()), _mode(rhs.mode()) {}
    
    int number() const {
        return _number;
    }
    void setNumber(int number) {
        _number = number;
    }
    
    bool printObject() const {
        return _printObject;
    }
    void setPrintObject(bool printObject) {
        _printObject = printObject;
    }
    
    int cancel() const {
        return _cancel;
    }
    void setCancel(int cancel) {
        _cancel = cancel;
    }
    
    int fifths() const {
        return _fifths;
    }
    void setFifths(int fifths) {
        _fifths = fifths;
    }
    
    Mode mode() const {
        return _mode;
    }
    void setMode(Mode mode) {
        _mode = mode;
    }

    /**
     Return the key's chromatic alteration for a particual note step.
     */
    int alter(Pitch::Step step) const;
    
public:
    int _number;
    bool _printObject;
    
    int _cancel;
    int _fifths;
    Mode _mode;
};

} // namespace dom
} // namespace mxml
