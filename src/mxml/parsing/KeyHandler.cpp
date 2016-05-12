// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "KeyHandler.h"
#include <mxml/dom/InvalidDataError.h>

namespace mxml {

using lxml::QName;
using dom::Key;

static const char* kNumberAttribute = "number";
static const char* kPrintObjectAttribute = "print-object";
static const char* kCancelTag = "cancel";
static const char* kFifthsTag = "fifths";
static const char* kModeTag = "mode";

void KeyHandler::startElement(const QName& qname, const AttributeMap& attributes) {
    _result.reset(new Key());
    
    auto number = attributes.find(kNumberAttribute);
    if (number != attributes.end())
        _result->setNumber(lxml::IntegerHandler::parseInteger(number->second));
    
    auto print = attributes.find(kPrintObjectAttribute);
    if (print != attributes.end())
        _result->setPrintObject(print->second == "no" ? false : true);
}

lxml::RecursiveHandler* KeyHandler::startSubElement(const QName& qname) {
    if (strcmp(qname.localName(), kCancelTag) == 0)
        return &_integerHandler;
    else if (strcmp(qname.localName(), kFifthsTag) == 0)
        return &_integerHandler;
    else if (strcmp(qname.localName(), kModeTag) == 0)
        return &_stringHandler;
    return 0;
}

void KeyHandler::endSubElement(const QName& qname, RecursiveHandler* parser) {
    if (strcmp(qname.localName(), kCancelTag) == 0)
        _result->setCancel(_integerHandler.result());
    else if (strcmp(qname.localName(), kFifthsTag) == 0)
        _result->setFifths(_integerHandler.result());
    else if (strcmp(qname.localName(), kModeTag) == 0)
        _result->setMode(modeFromString(_stringHandler.result()));
}

Key::Mode KeyHandler::modeFromString(const std::string& string) {
    if (string == "major")
        return Key::Mode::Major;
    else if (string == "minor")
        return Key::Mode::Minor;
    else if (string == "dorian")
        return Key::Mode::Dorian;
    else if (string == "phrygian")
        return Key::Mode::Phrygian;
    else if (string == "lydian")
        return Key::Mode::Lydian;
    else if (string == "mixolydian")
        return Key::Mode::Mixolydian;
    else if (string == "aeolian")
        return Key::Mode::Aeolian;
    else if (string == "ionian")
        return Key::Mode::Ionian;
    else if (string == "locrian")
        return Key::Mode::Locrian;
    else if (string == "none")
        return Key::Mode::None;
    throw dom::InvalidDataError("Invalid key mode " + string);
}

} // namespace mxml
