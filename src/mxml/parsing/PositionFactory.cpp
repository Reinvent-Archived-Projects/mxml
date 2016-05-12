// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "PositionFactory.h"
#include <lxml/DoubleHandler.h>

static const char* kDefaultXAttribute = "default-x";
static const char* kDefaultYAttribute = "default-y";
static const char* kRelativeXAttribute = "relative-x";
static const char* kRelativeYAttribute = "relative-y";


namespace mxml {
namespace parsing {

dom::Position PositionFactory::buildFromAttributes(const lxml::RecursiveHandler::AttributeMap& attributes) {
    dom::Position position;

    using Iter = lxml::RecursiveHandler::AttributeMap::const_iterator;
    Iter it;

    it = attributes.find(kDefaultXAttribute);
    if (it != attributes.end())
        position.defaultX = value(it->second);

    it = attributes.find(kDefaultYAttribute);
    if (it != attributes.end())
        position.defaultY = value(it->second);

    it = attributes.find(kRelativeXAttribute);
    if (it != attributes.end())
        position.relativeX = value(it->second);

    it = attributes.find(kRelativeYAttribute);
    if (it != attributes.end())
        position.relativeY = value(it->second);

    return position;
}

dom::Position PositionFactory::buildFromGenericNode(const GenericNode& node) {
    dom::Position position;
    dom::Optional<std::string> string;

    string = node.attribute(kDefaultXAttribute);
    if (string)
        position.defaultX = value(string.value());
    
    string = node.attribute(kDefaultXAttribute);
    if (string)
        position.defaultX = value(string.value());

    string = node.attribute(kDefaultYAttribute);
    if (string)
        position.defaultY = value(string.value());

    string = node.attribute(kRelativeXAttribute);
    if (string)
        position.relativeX = value(string.value());

    string = node.attribute(kRelativeYAttribute);
    if (string)
        position.relativeY = value(string.value());

    return position;
}

dom::Optional<dom::tenths_t> PositionFactory::value(const std::string& string) {
    auto value = static_cast<dom::tenths_t>(lxml::DoubleHandler::parseDouble(string));
    return dom::presentOptional(value);
}

} // namespace parsing
} // namespace mxml
