// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "LyricHandler.h"
#include <lxml/IntegerHandler.h>
#include <mxml/dom/InvalidDataError.h>

namespace mxml {

static const char* kNumberAttribute = "number";
static const char* kNameAttribute = "name";
static const char* kPrintObjectAttribute = "name";

//static const char* kExtendTag = "extend";
static const char* kSyllabicTag = "syllabic";
static const char* kTextTag = "text";

using dom::Lyric;

void LyricHandler::startElement(const lxml::QName& qname, const AttributeMap& attributes) {
    _result.reset(new Lyric());

    auto number = attributes.find(kNumberAttribute);
    if (number != attributes.end())
        _result->setNumber(lxml::IntegerHandler::parseInteger(number->second));

    auto name = attributes.find(kNameAttribute);
    if (name != attributes.end())
        _result->setName(name->second);

    auto print = attributes.find(kPrintObjectAttribute);
    if (print != attributes.end())
        _result->setPrintObject(print->second == "no" ? false : true);
}

lxml::RecursiveHandler* LyricHandler::startSubElement(const lxml::QName& qname) {
    if (strcmp(qname.localName(), kSyllabicTag) == 0)
        return &_syllabicHandler;
    else if (strcmp(qname.localName(), kTextTag) == 0)
        return &_stringHandler;
    return 0;
}

void LyricHandler::endSubElement(const lxml::QName& qname, RecursiveHandler* parser) {
    if (strcmp(qname.localName(), kSyllabicTag) == 0)
        _result->setSyllabic(_syllabicHandler.result());
    else if (strcmp(qname.localName(), kTextTag) == 0)
        _result->setText(_stringHandler.result());
}

} // namespace
