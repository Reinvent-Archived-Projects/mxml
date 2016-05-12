// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <lxml/DoubleHandler.h>

#include "CreditWordsHandler.h"
#include "PositionFactory.h"
#include "TypeFactories.h"


namespace mxml {

using namespace parsing;
using dom::CreditWords;
using lxml::QName;

static const char* kFontFamiltyAttribute = "font-family";
static const char* kFontStyleAttribute = "font-style";
static const char* kFontWeightAttribute = "font-weight";
static const char* kFontSizeAttribute = "font-size";

void CreditWordsHandler::startElement(const QName& qname, const AttributeMap& attributes) {
    using lxml::DoubleHandler;
    
    _result.reset(new CreditWords());
    _result->position = PositionFactory::buildFromAttributes(attributes);
    _result->justify = JustifyFactory::buildFromAttributes(attributes);

    auto fontFamily = attributes.find(kFontFamiltyAttribute);
    if (fontFamily != attributes.end())
        _result->setFontFamily(fontFamily->second);
    
    auto fontStyle = attributes.find(kFontStyleAttribute);
    if (fontStyle != attributes.end())
        _result->setFontStyle(fontStyleFromString(fontStyle->second));
    
    auto fontWeight = attributes.find(kFontWeightAttribute);
    if (fontWeight != attributes.end())
        _result->setFontWeight(fontWeightFromString(fontWeight->second));
    
    auto fontSize = attributes.find(kFontSizeAttribute);
    if (fontSize != attributes.end())
        _result->setFontSize(DoubleHandler::parseDouble(fontSize->second));
}

void CreditWordsHandler::endElement(const QName& qname, const std::string& contents) {
    _result->contents = contents;
}

CreditWords::FontStyle CreditWordsHandler::fontStyleFromString(const std::string& string) {
    if (string == "italic")
        return CreditWords::FontStyle::Italic;
    return CreditWords::FontStyle::Normal;
}

CreditWords::FontWeight CreditWordsHandler::fontWeightFromString(const std::string& string) {
    if (string == "bold")
        return CreditWords::FontWeight::Bold;
    return CreditWords::FontWeight::Normal;
}

} // namespace mxml
