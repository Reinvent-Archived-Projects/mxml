//  Created by Alejandro Isaza on 2014-05-13.
//  Copyright (c) 2014 Venture Media Labs. All rights reserved.

#pragma once
#include "Node.h"
#include "Optional.h"

namespace mxml {
namespace dom {

class Key : public Node {
public:
    enum Mode {
        MODE_MAJOR,
        MODE_MINOR,
        MODE_DORIAN,
        MODE_PHRYGIAN,
        MODE_LYDIAN,
        MODE_MIXOLYDIAN,
        MODE_AEOLIAN,
        MODE_IONIAN,
        MODE_LOCRAIN,
        MODE_NONE
    };
    
public:
    Key() : _number(1), _printObject(true), _cancel(), _fifths(), _mode(MODE_MAJOR) {}
    
    int number() const {
        return _number;
    }
    void setNumber(int number) {
        _number = number;
    }
    
    bool printObject() {
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
    
private:
    int _number;
    bool _printObject;
    
    int _cancel;
    int _fifths;
    Mode _mode;
};

} // namespace dom
} // namespace mxml
