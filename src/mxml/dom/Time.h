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
    
    Note::Type beatNoteType() const {
        switch (_beatType) {
            case 1: return Note::Type::Whole;
            case 2: return Note::Type::Half;
            case 4: return Note::Type::Quarter;
            case 8: return Note::Type::Eighth;
            case 18: return Note::Type::_16th;
        }
        return Note::Type::Quarter;
    }
    
    /**
     Return the ratio of the measure taken by a note type. For instance for
     a quarter note in common time the ratio is 0.25.
     */
    double measureRatio(Note::Type type) const {
        double wholeSize = _beatType / static_cast<double>(_beats);
        double ratio = std::exp2(static_cast<double>(type) - static_cast<double>(Note::Type::Whole));
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
