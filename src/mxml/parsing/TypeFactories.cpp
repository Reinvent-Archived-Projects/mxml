// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "TypeFactories.h"

#include <lxml/DoubleHandler.h>
#include <mxml/dom/InvalidDataError.h>
#include <cstring>


namespace mxml {
namespace parsing {

bool Factory::yesNoValue(const std::string& string) {
    if (string == "yes")
        return true;
    if (string == "no")
        return false;

    throw dom::InvalidDataError("Invalid print-object value " + string);
}

dom::tenths_t Factory::tenthsValue(const std::string& string) {
    return static_cast<dom::tenths_t>(lxml::DoubleHandler::parseDouble(string));
}

dom::Optional<dom::Justify> JustifyFactory::buildFromAttributes(const AttributeMap& attributes) {
    auto it = attributes.find("justify");
    if (it != attributes.end())
        return value(it->second);

    return dom::absentOptional(dom::Justify::Left);
}

dom::Optional<dom::Justify> JustifyFactory::buildFromGenericNode(const GenericNode& node) {
    auto string = node.attribute("justify");
    if (string)
        return value(string.value());

    return dom::absentOptional(dom::Justify::Left);
}

dom::Optional<dom::Justify> JustifyFactory::value(const std::string& string) {
    if (string == "left")
        return dom::Justify::Left;
    else if (string == "center")
        return dom::Justify::Center;
    else if (string == "right")
        return dom::Justify::Right;
    
    throw dom::InvalidDataError("Invalid justify type " + string);
}

dom::Optional<dom::HAlign> HAlignFactory::buildFromAttributes(const AttributeMap& attributes) {
    auto it = attributes.find("halign");
    if (it != attributes.end())
        return value(it->second);

    return dom::absentOptional(dom::HAlign::Left);
}

dom::Optional<dom::HAlign> HAlignFactory::buildFromGenericNode(const GenericNode& node) {
    auto string = node.attribute("halign");
    if (string)
        return value(string.value());

    return dom::absentOptional(dom::HAlign::Left);
}

dom::Optional<dom::HAlign> HAlignFactory::value(const std::string& string) {
    if (string == "left")
        return dom::HAlign::Left;
    else if (string == "center")
        return dom::HAlign::Center;
    else if (string == "right")
        return dom::HAlign::Right;

    throw dom::InvalidDataError("Invalid halign type " + string);
}

dom::Optional<dom::VAlign> VAlignFactory::buildFromAttributes(const AttributeMap& attributes) {
    auto it = attributes.find("valign");
    if (it != attributes.end())
        return value(it->second);

    return dom::absentOptional(dom::VAlign::Top);
}

dom::Optional<dom::VAlign> VAlignFactory::buildFromGenericNode(const GenericNode& node) {
    auto string = node.attribute("valign");
    if (string)
        return value(string.value());

    return dom::absentOptional(dom::VAlign::Top);
}

dom::Optional<dom::VAlign> VAlignFactory::value(const std::string& string) {
    if (string == "top")
        return dom::VAlign::Top;
    else if (string == "middle")
        return dom::VAlign::Middle;
    else if (string == "bottom")
        return dom::VAlign::Bottom;
    else if (string == "baseline")
        return dom::VAlign::Baseline;

    throw dom::InvalidDataError("Invalid valign type " + string);
}

bool PrintObjectFactory::buildFromAttributes(const AttributeMap& attributes) {
    auto it = attributes.find("print-object");
    if (it != attributes.end())
        return yesNoValue(it->second);
    return true;
}

bool PrintObjectFactory::buildFromGenericNode(const GenericNode& node) {
    auto string = node.attribute("print-object");
    if (string)
        return yesNoValue(string.value());
    return true;
}

} // namespace parsing
} // namespace mxml
