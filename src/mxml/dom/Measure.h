//  Created by Alejandro Isaza on 2/17/2014.
//  Copyright (c) 2014 Venture Media Labs Inc. All rights reserved.

#pragma once
#include "Attributes.h"
#include "Node.h"

#include <functional>
#include <map>
#include <memory>
#include <string>
#include <vector>

using std::shared_ptr;
using std::unique_ptr;
using std::vector;

namespace mxml {
namespace dom {

class Part;

class Measure : public Node {
public:
    typedef std::map<int, vector<shared_ptr<TimedNode>>> timed_node_map_t;
    
public:
    Measure();
    
    const std::string& number() const {
        return _number;
    }
    void setNumber(const std::string& number) {
        _number = number;
    }
    
    const unique_ptr<Attributes>& baseAttributes() const {
        return _baseAttributes;
    }
    void setBaseAttributes(unique_ptr<Attributes> attr) {
        _baseAttributes = std::move(attr);
    }
    
    const std::vector<unique_ptr<Node>>& nodes() const {
        return _nodes;
    }
    void addNode(unique_ptr<Node>&& node) {
        node->setParent(this);
        _nodes.push_back(std::move(node));
    }
    
    /** Get the smallest division gap between consecutive timed nodes.
     */
    time_t smallestTimeDelta() const;
    
    /** Get the total duration of the measure in divisions.
     */
    time_t duration() const;
    
private:
    std::string _number;
    unique_ptr<Attributes> _baseAttributes;
    vector<unique_ptr<Node>> _nodes;
};

} // namespace dom
} // namespace mxml
