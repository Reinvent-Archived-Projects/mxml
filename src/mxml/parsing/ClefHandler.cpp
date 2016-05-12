// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "ClefHandler.h"
#include <mxml/dom/InvalidDataError.h>
#include <cstring>

namespace mxml {

using dom::Clef;
using lxml::QName;

static const char* kNumberAttribute = "number";
static const char* kSignTag = "sign";
static const char* kLineTag = "line";

void ClefHandler::startElement(const QName& qname, const AttributeMap& attributes) {
    _result.reset(new Clef());
    
    auto it = attributes.find(kNumberAttribute);
    if (it != attributes.end())
        _result->setNumber(lxml::IntegerHandler::parseInteger(it->second));
}

lxml::RecursiveHandler* ClefHandler::startSubElement(const QName& qname) {
    if (strcmp(qname.localName(), kSignTag) == 0)
        return &_stringHandler;
    else if (strcmp(qname.localName(), kLineTag) == 0)
        return &_integerHandler;
    return 0;
}

void ClefHandler::endSubElement(const QName& qname, RecursiveHandler* parser) {
    if (strcmp(qname.localName(), kSignTag) == 0)
        _result->setSign(dom::presentOptional(signFromString(_stringHandler.result())));
    else if (strcmp(qname.localName(), kLineTag) == 0)
        _result->setLine(dom::presentOptional(_integerHandler.result()));
}

Clef::Sign ClefHandler::signFromString(const std::string& string) {
    if (string == "none") return Clef::Sign::None;
    if (string == "G") return Clef::Sign::G;
    if (string == "F") return Clef::Sign::F;
    if (string == "C") return Clef::Sign::C;
    if (string == "percussion") return Clef::Sign::Percussion;
    if (string == "tab") return Clef::Sign::Tab;
    if (string == "jianpu") return Clef::Sign::Jianpu;
    throw dom::InvalidDataError("Invalid clef sign " + string);
}

} // namespace mxml
