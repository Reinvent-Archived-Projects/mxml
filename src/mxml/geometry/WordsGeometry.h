//  Created by Alejandro Isaza on 2014-05-05.
//  Copyright (c) 2014 Venture Media Labs. All rights reserved.

#pragma once
#include "PlacementGeometry.h"
#include <mxml/dom/Direction.h>

namespace mxml {

class WordsGeometry : public PlacementGeometry {
public:
    explicit WordsGeometry(const dom::Direction& direction);
    
    const dom::Direction& direction() const {
        return _direction;
    }
    
    bool dynamics() const {
        return _dynamics;
    }
    
    const std::string* contents() const;

    static Size computeSize(const dom::Words& words);
    static Size computeSize(const dom::Dynamics& dynamics);
    
private:
    const dom::Direction& _direction;
    bool _dynamics;
};

} // namespace mxml
