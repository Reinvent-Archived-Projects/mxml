// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

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
    
    const std::vector<std::unique_ptr<TypedValue>>& creators() const {
        return _creators;
    }
    const std::string& creator(const std::string& type) const;
    void addCreator(std::unique_ptr<TypedValue> creator);
    
    const std::vector<std::unique_ptr<TypedValue>>& rights() const {
        return _rights;
    }
    const std::string& rights(const std::string& type) const;
    void addRights(std::unique_ptr<TypedValue> rights);
    
    const std::string& source() const {
        return _source;
    }
    void setSource(const std::string& source) {
        _source = source;
    }
    
private:
    std::vector<std::unique_ptr<TypedValue>> _creators;
    std::vector<std::unique_ptr<TypedValue>> _rights;
    std::string _source;
};

} // namespace dom
} // namespace mxml
