//  Created by Alejandro Isaza on 2/20/2014.
//  Copyright (c) 2014 Venture Media Labs Inc. All rights reserved.

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

    _result = Time();
    
    auto number = attributes.find(kNumberAttribute);
    if (number != attributes.end())
        _result.setNumber(presentOptional(lxml::IntegerHandler::parseInteger(number->second)));
    
    auto symbol = attributes.find(kSymbolAttribute);
    if (symbol != attributes.end())
        _result.setSymbol(symbolFromString(symbol->second));
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
        _result.setBeats(_integerHandler.result());
    else if (strcmp(qname.localName(), kBeatTypeTag) == 0)
        _result.setBeatType(_integerHandler.result());
    else if (strcmp(qname.localName(), kSenzaMisuraTag) == 0)
        _result.setSenzaMisura(presentOptional(_stringHandler.result()));
}

Time::Symbol TimeHandler::symbolFromString(const std::string& string) {
    if (string == "common")
        return Time::SYMBOL_COMMON;
    if (string == "cut")
        return Time::SYMBOL_CUT;
    if (string == "single-number")
        return Time::SYMBOL_SINGLE_NUMBER;
    if (string == "note")
        return Time::SYMBOL_NOTE;
    if (string == "dotted-note")
        return Time::SYMBOL_DOTTED_NOTE;
    return Time::SYMBOL_NORMAL;
}

} // namespace mxml
