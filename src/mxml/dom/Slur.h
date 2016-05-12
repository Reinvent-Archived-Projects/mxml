// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once
#include "Node.h"
#include "Types.h"

namespace mxml {
namespace dom {

class Slur : public Node {
public:
    Slur() : _number(1), _type(), _placement(Placement::Below), _orientation(Orientation::Under) {}
    
    int number() const {
        return _number;
    }
    void setNumber(int number) {
        _number = number;
    }
    
    StartStopContinue type() const {
        return _type;
    }
    void setType(StartStopContinue type) {
        _type = type;
    }
    
    Placement placement() const {
        return _placement;
    }
    void setPlacement(Placement placement) {
        _placement = placement;
    }
    
    Orientation orientation() const {
        return _orientation;
    }
    void setOrientation(Orientation orientation) {
        _orientation = orientation;
    }
    
private:
    int _number;
    StartStopContinue _type;
    Placement _placement;
    Orientation _orientation;
};

} // namespace dom
} // namespace mxml
