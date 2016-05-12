// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "GenericNode.h"

namespace mxml {
namespace parsing {

dom::Optional<std::string> GenericNode::attribute(const std::string& string) const {
    auto it = _attributes.find(string);
    if (it != _attributes.end())
        return it->second;
    return dom::Optional<std::string>();
}

void GenericNode::setAttribute(const std::string& name, const std::string& value) {
    _attributes[name] = value;
}

const GenericNode* GenericNode::child(const std::string& name) const {
    for (auto& child : _children) {
        if (child->name() == name)
            return child.get();
    }
    return nullptr;
}

} // namespace parsing
} // namespace mxml
