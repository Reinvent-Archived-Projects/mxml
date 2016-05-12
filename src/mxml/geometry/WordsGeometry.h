// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

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
