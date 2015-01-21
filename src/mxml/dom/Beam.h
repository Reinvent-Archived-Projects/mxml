//  Created by Alejandro Isaza on 2014-03-04.
//  Copyright (c) 2014 Venture Media Labs Inc. All rights reserved.

#pragma once
#include "Node.h"

namespace mxml {
namespace dom {

class Beam : public Node {
public:
    enum Type {
        TYPE_BEGIN,
        kContinue,
        TYPE_END,
        TYPE_FORWARD_HOOK,
        TYPE_BACKWARD_HOOK
    };
    
public:
    int number() const {
        return _number;
    }
    void setNumber(int number) {
        _number = number;
    }
    
    Type type() const {
        return _type;
    }
    void setType(Type type) {
        _type = type;
    }

private:
    int _number;
    Type _type;
};

} // namespace dom
} // namespace mxml
