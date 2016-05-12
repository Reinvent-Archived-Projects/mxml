// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once
#include "Node.h"
#include "Optional.h"

#include <cmath>
#include <string>


namespace mxml {
namespace dom {

class Time : public Node {
public:
    enum class Symbol {
        Normal,
        Common,
        Cut,
        SingleNumber,
        Note,
        DottedNote
    };
    
public:
    Time() : _number(1), _symbol(Symbol::Normal), _senzaMisura(), _beats(4), _beatType(4) {}
    
    const Optional<int>& number() const {
        return _number;
    }
    void setNumber(const Optional<int>& number) {
        _number = number;
    }
    
    Symbol symbol() const {
        return _symbol;
    }
    void setSymbol(Symbol symbol) {
        _symbol = symbol;
    }
    
    const Optional<std::string>& senzaMisura() const {
        return _senzaMisura;
    }
    void setSenzaMisura(const Optional<std::string>& senzaMisura) {
        _senzaMisura = senzaMisura;
    }
    
    int beats() const {
        return _beats;
    }
    void setBeats(int beats) {
        _beats = beats;
    }
    
    int beatType() const {
        return _beatType;
    }
    void setBeatType(int beatType) {
        _beatType = beatType;
    }
    
private:
    Optional<int> _number;
    Symbol _symbol;
    Optional<std::string> _senzaMisura;
    int _beats;
    int _beatType;
};

} // namespace dom
} // namespace mxml
