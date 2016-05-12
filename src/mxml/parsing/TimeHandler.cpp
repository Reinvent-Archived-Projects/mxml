// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "TimeHandler.h"
#include <mxml/dom/InvalidDataError.h>
#include <cstring>

namespace mxml {

using dom::Time;
using lxml::QName;

static const char* kNumberAttribute = "number";
static const char* kSymbolAttribute = "symbol";
static const char* kSenzaMisuraTag = "senza-misura";
static const char* kBeatsTag = "beats";
static const char* kBeatTypeTag = "beat-type";

void TimeHandler::startElement(const QName& qname, const AttributeMap& attributes) {
    using dom::presentOptional;

    _result.reset(new Time());
    
    auto number = attributes.find(kNumberAttribute);
    if (number != attributes.end())
        _result->setNumber(presentOptional(lxml::IntegerHandler::parseInteger(number->second)));
    
    auto symbol = attributes.find(kSymbolAttribute);
    if (symbol != attributes.end())
        _result->setSymbol(symbolFromString(symbol->second));
}

lxml::RecursiveHandler* TimeHandler::startSubElement(const QName& qname) {
    if (strcmp(qname.localName(), kBeatsTag) == 0)
        return &_integerHandler;
    else if (strcmp(qname.localName(), kBeatTypeTag) == 0)
        return &_integerHandler;
    else if (strcmp(qname.localName(), kSenzaMisuraTag) == 0)
        return &_stringHandler;
    return 0;
}

void TimeHandler::endSubElement(const QName& qname, RecursiveHandler* parser) {
    using dom::presentOptional;

    if (strcmp(qname.localName(), kBeatsTag) == 0)
        _result->setBeats(_integerHandler.result());
    else if (strcmp(qname.localName(), kBeatTypeTag) == 0)
        _result->setBeatType(_integerHandler.result());
    else if (strcmp(qname.localName(), kSenzaMisuraTag) == 0)
        _result->setSenzaMisura(presentOptional(_stringHandler.result()));
}

Time::Symbol TimeHandler::symbolFromString(const std::string& string) {
    if (string == "common")
        return Time::Symbol::Common;
    if (string == "cut")
        return Time::Symbol::Cut;
    if (string == "single-number")
        return Time::Symbol::SingleNumber;
    if (string == "note")
        return Time::Symbol::Note;
    if (string == "dotted-note")
        return Time::Symbol::DottedNote;
    return Time::Symbol::Normal;
}

} // namespace mxml
