//  Copyright (c) 2015 Venture Media Labs. All rights reserved.

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

    return dom::absentOptional(dom::kJustifyLeft);
}

dom::Optional<dom::Justify> JustifyFactory::buildFromGenericNode(const GenericNode& node) {
    auto string = node.attribute("justify");
    if (string)
        return value(string.value());

    return dom::absentOptional(dom::kJustifyLeft);
}

dom::Optional<dom::Justify> JustifyFactory::value(const std::string& string) {
    if (string == "left")
        return dom::kJustifyLeft;
    else if (string == "center")
        return dom::kJustifyCenter;
    else if (string == "right")
        return dom::kJustifyRight;
    
    throw dom::InvalidDataError("Invalid justify type " + string);
}

dom::Optional<dom::HAlign> HAlignFactory::buildFromAttributes(const AttributeMap& attributes) {
    auto it = attributes.find("halign");
    if (it != attributes.end())
        return value(it->second);

    return dom::absentOptional(dom::kHAlignLeft);
}

dom::Optional<dom::HAlign> HAlignFactory::buildFromGenericNode(const GenericNode& node) {
    auto string = node.attribute("halign");
    if (string)
        return value(string.value());

    return dom::absentOptional(dom::kHAlignLeft);
}

dom::Optional<dom::HAlign> HAlignFactory::value(const std::string& string) {
    if (string == "left")
        return dom::kHAlignLeft;
    else if (string == "center")
        return dom::kHalignCenter;
    else if (string == "right")
        return dom::kHAlignRight;

    throw dom::InvalidDataError("Invalid halign type " + string);
}

dom::Optional<dom::VAlign> VAlignFactory::buildFromAttributes(const AttributeMap& attributes) {
    auto it = attributes.find("valign");
    if (it != attributes.end())
        return value(it->second);

    return dom::absentOptional(dom::kVAlignTop);
}

dom::Optional<dom::VAlign> VAlignFactory::buildFromGenericNode(const GenericNode& node) {
    auto string = node.attribute("valign");
    if (string)
        return value(string.value());

    return dom::absentOptional(dom::kVAlignTop);
}

dom::Optional<dom::VAlign> VAlignFactory::value(const std::string& string) {
    if (string == "top")
        return dom::kVAlignTop;
    else if (string == "middle")
        return dom::kVAlignMiddle;
    else if (string == "bottom")
        return dom::kVAlignBottom;
    else if (string == "baseline")
        return dom::kVAlignBaseline;

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
