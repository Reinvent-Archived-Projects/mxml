//  Created by Alejandro Isaza on 2014-04-23.
//  Copyright (c) 2014 Venture Media Labs. All rights reserved.

#include "FermataHandler.h"
#include <mxml/dom/InvalidDataError.h>

namespace mxml {

using lxml::QName;
using dom::InvalidDataError;
using dom::Fermata;

static const char* kTypeAttribute = "type";

void FermataHandler::startElement(const QName& qname, const AttributeMap& attributes) {
    _result.reset(new Fermata());
    
    auto type = attributes.find(kTypeAttribute);
    if (type != attributes.end())
        _result->setType(typeFromString(type->second));
}

void FermataHandler::endElement(const QName& qname, const std::string& contents) {
    if (contents.empty())
        _result->setShape(Fermata::Shape::Normal);
    else
        _result->setShape(shapeFromString(contents));
}

Fermata::Type FermataHandler::typeFromString(const std::string& string) {
    if (string == "upright")
        return Fermata::Type::Upright;
    else if (string == "inverted")
        return Fermata::Type::Inverted;
    throw InvalidDataError("Invalid fermata type " + string);
}

Fermata::Shape FermataHandler::shapeFromString(const std::string& string) {
    if (string == "normal")
        return Fermata::Shape::Normal;
    else if (string == "angled")
        return Fermata::Shape::Angled;
    else if (string == "square")
        return Fermata::Shape::Square;
    throw InvalidDataError("Invalid fermata shape " + string);
}

} // namespace mxml
