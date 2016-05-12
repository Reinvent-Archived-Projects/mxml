// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once
#include "Measure.h"
#include "Node.h"

#include <memory>
#include <vector>

namespace mxml {
namespace dom {

class Score;

class Part : public Node {
public:
    Part() : _id(), _name(), _measures() {}
    explicit Part(const std::string& id) : _id(id) {}

    /**
     The part's index. The first part has index 0.
     */
    std::size_t index() const {
        return _index;
    }
    void setIndex(std::size_t i) {
        _index = i;
    }
    
    const std::string& id() const {
        return _id;
    }
    void setId(const std::string& id) {
        _id = id;
    }
    void setId(std::string&& id) {
        _id = id;
    }
    
    const std::string& name() const {
        return _name;
    }
    void setName(const std::string& name) {
        _name = name;
    }
    void setName(std::string&& name) {
        _name = name;
    }
    
    const std::vector<std::unique_ptr<Measure>>& measures() const {
        return _measures;
    }
    const Measure* measureWithNumber(const std::string& number) const;
    void addMeasure(std::unique_ptr<Measure>&& measure) {
        _measures.push_back(std::move(measure));
    }
    
private:
    std::size_t _index;
    std::string _id;
    std::string _name;
    std::vector<std::unique_ptr<Measure>> _measures;
};

} // namespace dom
} // namespace mxml
