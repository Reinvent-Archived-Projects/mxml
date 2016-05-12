// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once
#include "Node.h"
#include "Optional.h"

#include <memory>

namespace mxml {
namespace dom {

class Clef : public Node {
public:
    enum class Sign : int {
        None,
        G,
        F,
        C,
        Percussion,
        Tab,
        Jianpu,
    };
    
    static std::unique_ptr<Clef> trebleClef(int number = 1) {
        std::unique_ptr<Clef> clef(new Clef(number));
        clef->setSign(presentOptional(Clef::Sign::G));
        clef->setLine(presentOptional(2));
        return clef;
    }
    
    static std::unique_ptr<Clef> bassClef(int number = 1) {
        std::unique_ptr<Clef> clef(new Clef(number));
        clef->setSign(presentOptional(Clef::Sign::F));
        clef->setLine(presentOptional(4));
        return clef;
    }
    
public:
    Clef() : _number(1), _sign(Sign::G), _line() {}
    Clef(int number) : _number(number), _sign(Sign::G), _line() {}
    
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
