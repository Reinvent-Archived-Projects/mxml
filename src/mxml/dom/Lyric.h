//  Created by Alejandro Isaza on 2014-07-09.
//  Copyright (c) 2014 Venture Media Labs. All rights reserved.

#pragma once
#include "Node.h"
#include "Optional.h"
#include "Syllabic.h"
#include "Types.h"

#include <string>

namespace mxml {
namespace dom {

class Lyric : public Node {
public:
    Lyric() : _number(1), _name(), _placement(PLACEMENT_BELOW), _printObject(true) {}

    int number() const {
        return _number;
    }
    void setNumber(int number) {
        _number = number;
    }

    const std::string& name() const {
        return _name;
    }
    void setName(const std::string& name) {
        _name = name;
    }

    Placement placement() const {
        return _placement;
    }
    void setPlacement(Placement placement) {
        _placement = placement;
    }

    bool printObject() const {
        return _printObject;
    }
    void setPrintObject(bool print) {
        _printObject = print;
    }

    const Optional<StartStopContinue>& extend() const {
        return _extend;
    }
    void setExtend(const Optional<StartStopContinue>& extend) {
        _extend = extend;
    }

    const Optional<Syllabic>& syllabic() const {
        return _syllabic;
    }
    void setSyllabic(const Optional<Syllabic>& syllabic) {
        _syllabic = syllabic;
    }

    const std::string& text() const {
        return _text;
    }
    void setText(const std::string& text) {
        _text = text;
    }
    
private:
    int _number;
    std::string _name;
    Placement _placement;
    bool _printObject;

    Optional<StartStopContinue> _extend;
    Optional<Syllabic> _syllabic;
    std::string _text;
};

} // namespace dom
} // namespace mxml
