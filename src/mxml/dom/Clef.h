//  Created by Alejandro Isaza on 2/19/2014.
//  Copyright (c) 2014 Venture Media Labs Inc. All rights reserved.

#pragma once
#include "Node.h"
#include "Optional.h"

namespace mxml {
namespace dom {

class Clef : public Node {
public:
    enum Sign {
        SIGN_NONE,
        SIGN_G,
        SIGN_F,
        SIGN_C,
        SIGN_PERCUSSION,
        SIGN_TAB,
        SIGN_JIANPU,
    };
    
    static Clef trebleClef(int number = 1) {
        Clef clef(number);
        clef.setSign(presentOptional(Clef::SIGN_G));
        clef.setLine(presentOptional(2));
        return clef;
    }
    
    static Clef bassClef(int number = 1) {
        Clef clef(number);
        clef.setSign(presentOptional(Clef::SIGN_F));
        clef.setLine(presentOptional(4));
        return clef;
    }
    
public:
    Clef() : _number(1), _sign(SIGN_NONE), _line() {}
    Clef(int number) : _number(number), _sign(SIGN_NONE), _line() {}
    
    int number() const {
        return _number;
    }
    void setNumber(int number) {
        _number = number;
    }
    
    const Optional<Sign>& sign() const {
        return _sign;
    }
    void setSign(const Optional<Sign>& sign) {
        _sign = sign;
    }
    
    const Optional<int>& line() const {
        return _line;
    }
    void setLine(const Optional<int>& line) {
        _line = line;
    }
    
private:
    int _number;
    Optional<Sign> _sign;
    Optional<int> _line;
};

} // namespace dom
} // namespace mxml
