// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <lxml/IntegerHandler.h>
#include <lxml/StringHandler.h>

#include "FormattedTextHandler.h"
#include "PositionFactory.h"
#include "TypeFactories.h"


namespace mxml {

using namespace parsing;

static const char* kUnderlineAttribute = "underline";
static const char* kOverlineAttribute = "overline";
static const char* kLineThroughAttribute = "line-through";

void FormattedTextHandler::startElement(const lxml::QName& qname, const AttributeMap& attributes) {
    _result = dom::FormattedText{};
    _result.position = PositionFactory::buildFromAttributes(attributes);
    _result.justify = JustifyFactory::buildFromAttributes(attributes);
    _result.horizontalAlignment = HAlignFactory::buildFromAttributes(attributes);
    _result.verticalAlignment = VAlignFactory::buildFromAttributes(attributes);

    AttributeMap::const_iterator it;

    it = attributes.find(kUnderlineAttribute);
    if (it != attributes.end())
        _result.underline = lxml::IntegerHandler::parseInteger(it->second) > 0;

    it = attributes.find(kOverlineAttribute);
    if (it != attributes.end())
        _result.overline = lxml::IntegerHandler::parseInteger(it->second) > 0;

    it = attributes.find(kLineThroughAttribute);
    if (it != attributes.end())
        _result.lineThrough = lxml::IntegerHandler::parseInteger(it->second) > 0;
}

void FormattedTextHandler::endElement(const lxml::QName& qname, const std::string& contents) {
    _result.string = lxml::StringHandler::trim(contents);
}

dom::FormattedText FormattedTextHandler::buildFromGenericNode(const parsing::GenericNode& node) {
    auto result = dom::FormattedText{};
    result.position = PositionFactory::buildFromGenericNode(node);
    result.justify = JustifyFactory::buildFromGenericNode(node);
    result.horizontalAlignment = HAlignFactory::buildFromGenericNode(node);
    result.verticalAlignment = VAlignFactory::buildFromGenericNode(node);

    dom::Optional<std::string> string;

    string = node.attribute(kUnderlineAttribute);
    if (string)
        result.underline = lxml::IntegerHandler::parseInteger(string) > 0;

    string = node.attribute(kOverlineAttribute);
    if (string)
        result.overline = lxml::IntegerHandler::parseInteger(string) > 0;

    string = node.attribute(kLineThroughAttribute);
    if (string)
        result.lineThrough = lxml::IntegerHandler::parseInteger(string) > 0;

    result.string = lxml::StringHandler::trim(node.text());
    return result;
}

} // namespace mxml
