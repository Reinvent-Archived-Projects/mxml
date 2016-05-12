// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once
#include <map>
#include <memory>
#include <string>
#include <vector>

#include <mxml/dom/Optional.h>


namespace mxml {
namespace parsing {

class GenericNode {
public:
    GenericNode() = default;
    explicit GenericNode(const std::string& name) : _name(name) {}

    const std::string& name() const { return _name; }
    void setName(const std::string& name) { _name = name; }

    bool hasAttribute(const std::string& string) const;
    dom::Optional<std::string> attribute(const std::string& string) const;
    void setAttribute(const std::string& name, const std::string& value);

    const std::string& text() const { return _text; }
    std::string& text() { return _text; }
    void setText(const std::string& string) { _text = string; }

    const GenericNode* parent() const { return _parent; }
    void setParent(const GenericNode* parent) { _parent = parent; }

    const std::vector<std::unique_ptr<GenericNode>>& children() const { return _children; }
    const std::unique_ptr<GenericNode>& child(std::size_t i) const { return _children[i]; }
    std::vector<std::unique_ptr<GenericNode>>& children() { return _children; }
    std::unique_ptr<GenericNode>& child(std::size_t i) { return _children[i]; }
    void addChild(std::unique_ptr<GenericNode> child) { _children.push_back(std::move(child)); }

    /**
     Return the first child with the given name, or nullptr if the child is not found.
     */
    const GenericNode* child(const std::string& name) const;

private:
    std::string _name;

    std::map<std::string, std::string> _attributes;
    std::string _text;

    const GenericNode* _parent;
    std::vector<std::unique_ptr<GenericNode>> _children;

};

} // namespace parsing
} // namespace mxml
