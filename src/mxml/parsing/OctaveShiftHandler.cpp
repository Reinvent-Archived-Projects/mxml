// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "OctaveShiftHandler.h"
#include <mxml/dom/InvalidDataError.h>

static const char* kTypeAttribute = "type";
static const char* kNumberAttribute = "number";
static const char* kSizeAttribute = "size";


namespace mxml {

using dom::OctaveShift;

void OctaveShiftHandler::startElement(const lxml::QName& qname, const AttributeMap& attributes) {
    _result.reset(new dom::OctaveShift());

    auto type = attributes.find(kTypeAttribute);
    if (type != attributes.end())
        _result->type = typeFromString(type->second);

    auto number = attributes.find(kNumberAttribute);
    if (number != attributes.end())
        _result->number = lxml::IntegerHandler::parseInteger(number->second);

    auto size = attributes.find(kSizeAttribute);
    if (size != attributes.end())
        _result->size = lxml::IntegerHandler::parseInteger(size->second);
    
}

OctaveShift::Type OctaveShiftHandler::typeFromString(const std::string& string) {
    if (string == "up")
        return OctaveShift::Type::Up;
    else if (string == "down")
        return OctaveShift::Type::Down;
    else if (string == "stop")
        return OctaveShift::Type::Stop;
    else if (string == "continue")
        return OctaveShift::Type::Continue;

    throw dom::InvalidDataError("Invalid octave-shift type " + string);
}

}
