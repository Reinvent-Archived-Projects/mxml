//  Created by Alejandro Isaza on 2/19/2014.
//  Copyright (c) 2014 Venture Media Labs Inc. All rights reserved.

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
    std::size_t indexOfMeasure(const Measure* measure) const;
    void addMeasure(std::unique_ptr<Measure>&& measure) {
        _measures.push_back(std::move(measure));
    }
    
    /** Get the staff distance for the part. Staff distance can be defined under either the defaults element on the score or the print element for a particular measure. Even though it doesn't make sense to have different staff distances for different measures. This method looks for the presence of the value on the deaults element of the score and on the first measure element. A definition in a measure takes presedence over a definition in defaults.
     */
    float staffDistance() const;
    
    /** Get the number of staves in the part.
     */
    int staves() const;
    
private:
    std::string _id;
    std::string _name;
    std::vector<std::unique_ptr<Measure>> _measures;
};

} // namespace dom
} // namespace mxml
