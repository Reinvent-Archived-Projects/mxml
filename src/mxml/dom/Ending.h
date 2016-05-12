// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once
#include "Node.h"

#include <set>
#include <string>

namespace mxml {
namespace dom {

class Ending : public Node {
public:
    enum class Type {
        Start,
        Stop,
        Discontinue
    };
    
public:
    Ending() : _type(), _numbers() {}
    
    const Type type() const {
        return _type;
    }
    void setType(Type type) {
        _type = type;
    }
    
    const std::set<int>& numbers() const {
        return _numbers;
    }
    void setNumbers(const std::set<int>& numbers) {
        _numbers = numbers;
    }
    
    const std::string& content() const {
        return _content;
    }
    void setContent(const std::string& content) {
        _content = content;
    }
    
private:
    Type _type;
    std::set<int> _numbers;
    std::string _content;
};

} // namespace dom
} // namespace mxml
