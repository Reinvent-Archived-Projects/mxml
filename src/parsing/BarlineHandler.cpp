//  Created by Alejandro Isaza on 2014-05-01.
//  Copyright (c) 2014 Venture Media Labs. All rights reserved.

#include "BarlineHandler.h"
#include "dom/InvalidDataError.h"

namespace mxml {

using dom::Barline;
using lxml::QName;

static const char* kStyleTag = "bar-style";
static const char* kLocationTag = "location";
static const char* kEndingTag = "ending";
static const char* kRepeatTag = "repeat";

void BarlineHandler::startElement(const QName& qname, const AttributeMap& attributes) {
    _result.reset(new Barline);
}

lxml::RecursiveHandler* BarlineHandler::startSubElement(const QName& qname) {
    if (strcmp(qname.localName(), kStyleTag) == 0)
        return &_stringHandler;
    else if (strcmp(qname.localName(), kLocationTag) == 0)
        return &_stringHandler;
    else if (strcmp(qname.localName(), kEndingTag) == 0)
        return &_endingHandler;
    else if (strcmp(qname.localName(), kRepeatTag) == 0)
        return &_repeatHandler;
    return 0;
}

void BarlineHandler::endSubElement(const QName& qname, RecursiveHandler* parser) {
    if (strcmp(qname.localName(), kStyleTag) == 0)
        _result->setStyle(styleFromString(_stringHandler.result()));
    else if (strcmp(qname.localName(), kLocationTag) == 0)
        _result->setLocation(locationFromString(_stringHandler.result()));
    else if (strcmp(qname.localName(), kEndingTag) == 0)
        _result->setEnding(presentOptional(_endingHandler.result()));
    else if (strcmp(qname.localName(), kRepeatTag) == 0)
        _result->setRepeat(presentOptional(_repeatHandler.result()));
}

Barline::Style BarlineHandler::styleFromString(const std::string& string) {
    if (string == "regular")
        return Barline::REGULAR;
    else if (string == "dotted")
        return Barline::DOTTED;
    else if (string == "dashed")
        return Barline::DASHED;
    else if (string == "heavy")
        return Barline::HEAVY;
    else if (string == "light-light")
        return Barline::LIGHT_LIGHT;
    else if (string == "light-heavy")
        return Barline::LIGHT_HEAVY;
    else if (string == "heavy-light")
        return Barline::HEAVY_LIGHT;
    else if (string == "heavy-heavy")
        return Barline::HEAVY_HEAVY;
    else if (string == "tick")
        return Barline::TICK;
    else if (string == "short")
        return Barline::SHORT;
    else if (string == "none")
        return Barline::NONE;
    throw dom::InvalidDataError("Invalid barline style " + string);
}

Barline::Location BarlineHandler::locationFromString(const std::string& string) {
    if (string == "left")
        return Barline::LEFT;
    else if (string == "middle")
        return Barline::MIDDLE;
    else if (string == "right")
        return Barline::RIGHT;
    throw dom::InvalidDataError("Invalid barline location " + string);
}

} // namespace mxml
