// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once
#include "Node.h"

namespace mxml {
namespace dom {

class Beam : public Node {
public:
    enum class Type {
        Begin,
        Continue,
        End,
        ForwardHook,
        BackwardHook
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
