//  Created by Alejandro Isaza on 2014-05-01.
//  Copyright (c) 2014 Venture Media Labs. All rights reserved.

#pragma once
#include "Node.h"

#include <set>
#include <string>

namespace mxml {
namespace dom {

class Ending : public Node {
public:
    enum Type {
        START,
        STOP,
        DISCONTINUE
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
