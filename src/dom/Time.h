//  Created by Alejandro Isaza on 2/19/2014.
//  Copyright (c) 2014 Venture Media Labs Inc. All rights reserved.

#pragma once
#include "Node.h"
#include "Note.h"
#include "Optional.h"

#include <cmath>

namespace mxml {
namespace dom {

class Time : public Node {
public:
    enum Symbol {
        SYMBOL_NORMAL,
        SYMBOL_COMMON,
        SYMBOL_CUT,
        SYMBOL_SINGLE_NUMBER,
        SYMBOL_NOTE,
        SYMBOL_DOTTED_NOTE
    };
    
public:
    Time() : _number(1), _symbol(SYMBOL_NORMAL), _senzaMisura(), _beats(4), _beatType(4) {}
    
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
    
    Note::Type beatNoteType() const {
        switch (_beatType) {
            case 1: return Note::TYPE_WHOLE;
            case 2: return Note::TYPE_HALF;
            case 4: return Note::TYPE_QUARTER;
            case 8: return Note::TYPE_EIGHTH;
            case 18: return Note::TYPE_16TH;
        }
        return Note::TYPE_QUARTER;
    }
    
    /**
     Return the ratio of the measure taken by a note type. For instance for
     a quarter note in common time the ratio is 0.25.
     */
    double measureRatio(Note::Type type) const {
        double wholeSize = _beatType / static_cast<double>(_beats);
        double ratio = std::exp2(static_cast<double>(type - Note::TYPE_WHOLE));
        return wholeSize * ratio;
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
