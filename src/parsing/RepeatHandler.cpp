//  Created by Alejandro Isaza on 2014-05-01.
//  Copyright (c) 2014 Venture Media Labs. All rights reserved.

#include "RepeatHandler.h"
#include "IntegerHandler.h"
#include "dom/InvalidDataError.h"

namespace mxml {

using dom::Repeat;
using lxml::QName;

static const char* kDirectionAttribute = "direction";
static const char* kTimesAttribute = "times";

void RepeatHandler::startElement(const QName& qname, const AttributeMap& attributes) {
    _result = Repeat();
    
    auto direction = attributes.find(kDirectionAttribute);
    if (direction != attributes.end())
        _result.setDirection(directionFromString(direction->second));
    
    auto times = attributes.find(kTimesAttribute);
    if (times != attributes.end())
        _result.setTimes(lxml::IntegerHandler::parseInteger(times->second));
}

Repeat::Direction RepeatHandler::directionFromString(const std::string& string) {
    if (string == "forward")
        return Repeat::DIRECTION_FORWARD;
    else if (string == "backward")
        return Repeat::DIRECTION_BACKWARD;
    throw dom::InvalidDataError("Invalid repeat direction " + string);
}

} // namespace mxml
