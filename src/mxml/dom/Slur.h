//  Created by Alejandro Isaza on 2014-04-23.
//  Copyright (c) 2014 Venture Media Labs. All rights reserved.

#pragma once
#include "Node.h"
#include "Types.h"

namespace mxml {
namespace dom {

class Slur : public Node {
public:
    Slur() : _number(1), _type(), _placement(kPlacementBelow), _orientation(kOrientationUnder) {}
    
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
