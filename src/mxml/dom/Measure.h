//  Created by Alejandro Isaza on 2/17/2014.
//  Copyright (c) 2014 Venture Media Labs Inc. All rights reserved.

#pragma once
#include "Node.h"
#include "TimedNode.h"

#include <map>
#include <memory>
#include <string>
#include <vector>

using std::shared_ptr;
using std::unique_ptr;
using std::vector;

namespace mxml {
namespace dom {

class Measure : public Node {
public:
    typedef std::map<int, vector<shared_ptr<TimedNode>>> timed_node_map_t;
    
public:
    Measure();

    /**
     The measure number or label. It is usually a number stating at 1, but it can be any arbitraty string.
     */
    const std::string& number() const {
        return _number;
    }
    void setNumber(const std::string& number) {
        _number = number;
    }
    
    const std::vector<unique_ptr<Node>>& nodes() const {
        return _nodes;
    }
    void addNode(unique_ptr<Node>&& node) {
        node->setParent(this);
        _nodes.push_back(std::move(node));
    }
    
private:
    std::string _number;
    vector<unique_ptr<Node>> _nodes;
};

} // namespace dom
} // namespace mxml
