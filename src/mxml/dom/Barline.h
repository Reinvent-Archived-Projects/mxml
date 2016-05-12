// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once
#include "Ending.h"
#include "Node.h"
#include "Optional.h"
#include "Repeat.h"

namespace mxml {
namespace dom {

class Barline : public Node {
public:
    enum class Style {
        Regular,
        Dotted,
        Dashed,
        Heavy,
        LightLight,
        LightHeavy,
        HeavyLight,
        HeavyHeavy,
        Tick,
        Short,
        None
    };
    
    enum class Location {
        Right,
        Left,
        Middle
    };
    
public:
    Barline() : _style(Style::Regular), _location(Location::Middle), _ending(), _repeat() {}
    
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
