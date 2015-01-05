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
    
    const std::unique_ptr<Ending>& ending() const {
        return _ending;
    }
    void setEnding(std::unique_ptr<Ending> ending) {
        _ending = std::move(ending);
    }
    
    const std::unique_ptr<Repeat>& repeat() const {
        return _repeat;
    }
    void setRepeat(std::unique_ptr<Repeat> repeat) {
        _repeat = std::move(repeat);
    }
    
private:
    Style _style;
    Location _location;
    std::unique_ptr<Ending> _ending;
    std::unique_ptr<Repeat> _repeat;
};

} // namespace dom
} // namespace mxml
