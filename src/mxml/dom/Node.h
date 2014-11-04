//  Created by Alejandro Isaza on 2014-04-28.
//  Copyright (c) 2014 Venture Media Labs. All rights reserved.

#pragma once

namespace mxml {
namespace dom {

class Node {
public:
    Node() : _parent() {}
    virtual ~Node() {}
    
    const Node* parent() const {
        return _parent;
    }
    void setParent(const Node* parent) {
        _parent = parent;
    }

private:
    const Node* _parent;
};

} // namespace dom
} // namespace mxml
