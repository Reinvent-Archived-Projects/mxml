//  Created by Alejandro Isaza on 2015-01-19.
//  Copyright (c) 2015 Venture Media Labs. All rights reserved.

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
        return OctaveShift::UP;
    else if (string == "down")
        return OctaveShift::DOWN;
    else if (string == "stop")
        return OctaveShift::STOP;
    else if (string == "continue")
        return OctaveShift::CONTINUE;

    throw dom::InvalidDataError("Invalid octave-shift type " + string);
}

}
