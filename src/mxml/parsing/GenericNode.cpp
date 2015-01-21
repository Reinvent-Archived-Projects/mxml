//  Copyright (c) 2015 Venture Media Labs. All rights reserved.

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
