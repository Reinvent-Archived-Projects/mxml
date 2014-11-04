//  Created by Alejandro Isaza on 2014-06-03.
//  Copyright (c) 2014 Venture Media Labs. All rights reserved.

#pragma once
#include <lxml/BaseRecursiveHandler.h>
#include <mxml/dom/Node.h>
#include <memory>

namespace mxml {

template <typename T>
class BaseHandler : public lxml::BaseRecursiveHandler<std::unique_ptr<T>> {
public:
    typedef T NodeType;

public:
    BaseHandler() : _parent() {}

    dom::Node* parent() const {
        return _parent;
    }
    void setParent(dom::Node* parent) {
        _parent = parent;
    }

private:
    dom::Node* _parent;
};

} // namespace
