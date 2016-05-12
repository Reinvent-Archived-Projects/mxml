// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

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
    _result.reset(new Attributes());
}

void AttributesHandler::endElement(const QName& qname, const std::string& contents) {
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
        std::unique_ptr<dom::Clef> clef = std::move(_clefHandler.result());
        clef->setParent(_result.get());
        _result->setClef(clef->number(), std::move(clef));
    } else if (strcmp(qname.localName(), kTimeTag) == 0) {
        std::unique_ptr<dom::Time> time = std::move(_timeHandler.result());
        time->setParent(_result.get());
        _result->setTime(std::move(time));
    } else if (strcmp(qname.localName(), kKeyTag) == 0) {
        std::unique_ptr<dom::Key> key = std::move(_keyHandler.result());
        key->setParent(_result.get());
        _result->setKey(key->number(), std::move(key));
    }
}

} // namespace mxml
