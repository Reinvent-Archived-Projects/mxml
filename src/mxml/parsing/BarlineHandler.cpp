// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "BarlineHandler.h"
#include <mxml/dom/InvalidDataError.h>

namespace mxml {

using dom::Barline;
using lxml::QName;

static const char* kStyleTag = "bar-style";
static const char* kLocationTag = "location";
static const char* kEndingTag = "ending";
static const char* kRepeatTag = "repeat";

void BarlineHandler::startElement(const QName& qname, const AttributeMap& attributes) {
    _result.reset(new Barline());
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
        _result->setEnding(_endingHandler.result());
    else if (strcmp(qname.localName(), kRepeatTag) == 0)
        _result->setRepeat(_repeatHandler.result());
}

Barline::Style BarlineHandler::styleFromString(const std::string& string) {
    if (string == "regular")
        return Barline::Style::Regular;
    else if (string == "dotted")
        return Barline::Style::Dotted;
    else if (string == "dashed")
        return Barline::Style::Dashed;
    else if (string == "heavy")
        return Barline::Style::Heavy;
    else if (string == "light-light")
        return Barline::Style::LightLight;
    else if (string == "light-heavy")
        return Barline::Style::LightHeavy;
    else if (string == "heavy-light")
        return Barline::Style::HeavyLight;
    else if (string == "heavy-heavy")
        return Barline::Style::HeavyHeavy;
    else if (string == "tick")
        return Barline::Style::Tick;
    else if (string == "short")
        return Barline::Style::Short;
    else if (string == "none")
        return Barline::Style::None;
    throw dom::InvalidDataError("Invalid barline style " + string);
}

Barline::Location BarlineHandler::locationFromString(const std::string& string) {
    if (string == "left")
        return Barline::Location::Left;
    else if (string == "middle")
        return Barline::Location::Middle;
    else if (string == "right")
        return Barline::Location::Right;
    throw dom::InvalidDataError("Invalid barline location " + string);
}

} // namespace mxml
