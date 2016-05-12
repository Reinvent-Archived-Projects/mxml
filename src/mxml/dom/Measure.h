// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once
#include "Node.h"
#include "TimedNode.h"

#include <map>
#include <memory>
#include <string>
#include <vector>


namespace mxml {
namespace dom {

class Part;

class Measure : public Node {
public:
    typedef std::map<int, std::vector<std::shared_ptr<TimedNode>>> timed_node_map_t;
    
public:
    Measure();

    /**
     The measure's index. The first measure has index 0.
     */
    std::size_t index() const {
        return _index;
    }
    void setIndex(std::size_t i) {
        _index = i;
    }

    /**
     The measure number or label. It is usually a number stating at 1, but it can be any arbitraty string.
     */
    const std::string& number() const {
        return _number;
    }
    void setNumber(const std::string& number) {
        _number = number;
    }
    
    const std::vector<std::unique_ptr<Node>>& nodes() const {
        return _nodes;
    }
    void addNode(std::unique_ptr<Node>&& node) {
        node->setParent(this);
        _nodes.push_back(std::move(node));
    }

    const Part* part() const {
        return reinterpret_cast<const Part*>(parent());
    }

private:
    std::size_t _index;
    std::string _number;
    std::vector<std::unique_ptr<Node>> _nodes;
};

} // namespace dom
} // namespace mxml
