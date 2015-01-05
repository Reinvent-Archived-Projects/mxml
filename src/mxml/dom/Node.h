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
    
protected:
    // Nodes are only copyable within the dom
    Node(const Node& rhs) = default;
    Node& operator=(const Node& rhs) = default;

    const Node* root() const {
        const Node* root = this;
        while (root->parent())
            root = root->parent();
        return root;
    }

private:
    const Node* _parent;
};

} // namespace dom
} // namespace mxml
