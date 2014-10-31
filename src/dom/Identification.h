//  Created by Alejandro Isaza on 2014-03-25.
//  Copyright (c) 2014 Venture Media Labs. All rights reserved.

#pragma once
#include "Node.h"

#include <string>
#include <vector>

namespace mxml {
namespace dom {

class TypedValue {
public:
    TypedValue() : _type(), _value() {}
    
    const std::string& type() const {
        return _type;
    }
    void setType(const std::string& type) {
        _type = type;
    }
    void setType(std::string&& type) {
        _type = type;
    }
    
    const std::string& value() const {
        return _value;
    }
    void setValue(const std::string& value) {
        _value = value;
    }
    void setName(std::string&& value) {
        _value = value;
    }
    
private:
    std::string _type;
    std::string _value;
};

class Identification : public Node {
public:
    
public:
    Identification() {}
    
    const std::vector<TypedValue>& creators() const {
        return _creators;
    }
    const std::string& creator(const std::string& type) const;
    void addCreator(const TypedValue& creator);
    
    const std::vector<TypedValue>& rights() const {
        return _rights;
    }
    const std::string& rights(const std::string& type) const;
    void addRights(const TypedValue& rights);
    
    const std::string& source() const {
        return _source;
    }
    void setSource(const std::string& source) {
        _source = source;
    }
    
private:
    std::vector<TypedValue> _creators;
    std::vector<TypedValue> _rights;
    std::string _source;
};

} // namespace dom
} // namespace mxml
