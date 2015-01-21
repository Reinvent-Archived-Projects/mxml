//  Created by Alejandro Isaza on 2014-04-17.
//  Copyright (c) 2014 Venture Media Labs. All rights reserved.

#pragma once
#include "Node.h"
#include "Position.h"


namespace mxml {
namespace dom {

/**
 The accidental type represents actual notated accidentals. Editorial and cautionary indications are indicated by
 attributes. Values for these attributes are "no" if not present. Specific graphic display such as parentheses,
 brackets, and size are controlled by the level-display attribute group.
 */
struct Accidental : public Node {
public:
    enum Type : int {
        kTypeDoubleFlat = -2,
        kTypeFlat,
        kTypeNatural,
        kTypeSharp,
        kTypeDoubleSharp
    };
    
public:
    Accidental() : type(kTypeSharp) {}
    Accidental(Type type) : type(type) {}
    
public:
    Type type;
    Position poition;
};

} // namespace dom
} // namespace mxml
