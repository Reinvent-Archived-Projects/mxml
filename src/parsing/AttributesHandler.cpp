//  Created by Alejandro Isaza on 2/20/2014.
//  Copyright (c) 2014 Venture Media Labs Inc. All rights reserved.

#include "AttributesHandler.h"
#include <cstring>

namespace mxml {

using dom::Attributes;
using lxml::QName;

static const char* kDivisionsTag = "divisions";
static const char* kStavesTag = "staves";
static const char* kClefTag = "clef";
static const char* kTimeTag = "time";
static const char* kKeyTag = "key";

void AttributesHandler::startElement(const QName& qname, const AttributeMap& attributes) {
    _result.reset(new Attributes(_defaultAttributes));
}

void AttributesHandler::endElement(const QName& qname, const std::string& contents) {
    _defaultAttributes = *_result;
    _defaultAttributes.reset();
}

lxml::RecursiveHandler* AttributesHandler::startSubElement(const QName& qname) {
    if (strcmp(qname.localName(), kDivisionsTag) == 0)
        return &_integerHandler;
    else if (strcmp(qname.localName(), kStavesTag) == 0)
        return &_integerHandler;
    else if (strcmp(qname.localName(), kClefTag) == 0)
        return &_clefHandler;
    else if (strcmp(qname.localName(), kTimeTag) == 0)
        return &_timeHandler;
    else if (strcmp(qname.localName(), kKeyTag) == 0)
        return &_keyHandler;
    return 0;
}

void AttributesHandler::endSubElement(const QName& qname, RecursiveHandler* parser) {
    using dom::presentOptional;

    if (strcmp(qname.localName(), kDivisionsTag) == 0)
        _result->setDivisions(presentOptional(_integerHandler.result()));
    else if (strcmp(qname.localName(), kStavesTag) == 0)
        _result->setStaves(presentOptional(_integerHandler.result()));
    else if (strcmp(qname.localName(), kClefTag) == 0) {
        dom::Clef clef = _clefHandler.result();
        clef.setParent(_result.get());
        _result->setClef(clef.number(), presentOptional(std::move(clef)));
    } else if (strcmp(qname.localName(), kTimeTag) == 0) {
        dom::Time time = _timeHandler.result();
        time.setParent(_result.get());
        _result->setTime(presentOptional(std::move(time)));
    } else if (strcmp(qname.localName(), kKeyTag) == 0) {
        dom::Key key = _keyHandler.result();
        key.setParent(_result.get());
        _result->setKey(key.number(), presentOptional(std::move(key)));
    }
}

} // namespace mxml
