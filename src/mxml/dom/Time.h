//  Created by Alejandro Isaza on 2/19/2014.
//  Copyright (c) 2014 Venture Media Labs Inc. All rights reserved.

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
