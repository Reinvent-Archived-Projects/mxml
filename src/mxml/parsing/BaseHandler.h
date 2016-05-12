// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

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
