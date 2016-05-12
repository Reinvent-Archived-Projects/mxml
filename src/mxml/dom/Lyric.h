// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once
#include "Node.h"
#include "Syllabic.h"
#include "Types.h"

#include <string>

namespace mxml {
namespace dom {

class Lyric : public Node {
public:
    Lyric() : _number(1), _name(), _placement(Placement::Below), _printObject(true) {}

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

    const std::unique_ptr<StartStopContinue>& extend() const {
        return _extend;
    }
    void setExtend(std::unique_ptr<StartStopContinue>&& extend) {
        _extend = std::move(extend);
    }

    const std::unique_ptr<Syllabic>& syllabic() const {
        return _syllabic;
    }
    void setSyllabic(std::unique_ptr<Syllabic>&& syllabic) {
        _syllabic = std::move(syllabic);
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

    std::unique_ptr<StartStopContinue> _extend;
    std::unique_ptr<Syllabic> _syllabic;
    std::string _text;
};

} // namespace dom
} // namespace mxml
