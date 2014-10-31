//  Created by Alejandro Isaza on 2014-05-01.
//  Copyright (c) 2014 Venture Media Labs. All rights reserved.

#pragma once
#include "Ending.h"
#include "Node.h"
#include "Optional.h"
#include "Repeat.h"

namespace mxml {
namespace dom {

class Barline : public Node {
public:
    enum Style {
        REGULAR,
        DOTTED,
        DASHED,
        HEAVY,
        LIGHT_LIGHT,
        LIGHT_HEAVY,
        HEAVY_LIGHT,
        HEAVY_HEAVY,
        TICK,
        SHORT,
        NONE
    };
    
    enum Location {
        RIGHT,
        LEFT,
        MIDDLE
    };
    
public:
    Barline() : _style(REGULAR), _location(MIDDLE), _ending(), _repeat() {}
    
    Style style() const {
        return _style;
    }
    void setStyle(Style style) {
        _style = style;
    }
    
    Location location() const {
        return _location;
    }
    void setLocation(Location location) {
        _location = location;
    }
    
    const Optional<Ending>& ending() const {
        return _ending;
    }
    void setEnding(const Optional<Ending>& ending) {
        _ending = ending;
    }
    
    const Optional<Repeat>& repeat() const {
        return _repeat;
    }
    void setRepeat(const Optional<Repeat>& repeat) {
        _repeat = repeat;
    }
    
private:
    Style _style;
    Location _location;
    Optional<Ending> _ending;
    Optional<Repeat> _repeat;
};

} // namespace dom
} // namespace mxml
