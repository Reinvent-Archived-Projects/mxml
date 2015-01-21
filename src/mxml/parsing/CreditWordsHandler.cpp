//  Created by Alejandro Isaza on 2014-03-25.
//  Copyright (c) 2014 Venture Media Labs. All rights reserved.

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
        return CreditWords::FONT_STYLE_ITALIC;
    return CreditWords::FONT_STYLE_NORMAL;
}

CreditWords::FontWeight CreditWordsHandler::fontWeightFromString(const std::string& string) {
    if (string == "bold")
        return CreditWords::FONT_WEIGHT_BOLD;
    return CreditWords::FONT_WEIGHT_NORMAL;
}

} // namespace mxml
