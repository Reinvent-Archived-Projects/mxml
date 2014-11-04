//  Created by Alejandro Isaza on 2014-03-24.
//  Copyright (c) 2014 Venture Media Labs. All rights reserved.

#include "BeamHandler.h"
#include <lxml/IntegerHandler.h>

namespace mxml {

using dom::Beam;
using lxml::QName;

static const char* kNumberAttribute = "number";

void BeamHandler::startElement(const QName& qname, const AttributeMap& attributes) {
    auto number = attributes.find(kNumberAttribute);
    if (number != attributes.end())
        _result.setNumber(lxml::IntegerHandler::parseInteger(number->second));
}

void BeamHandler::endElement(const QName& qname, const std::string& contents) {
    _result.setType(typeFromString(contents));
}

Beam::Type BeamHandler::typeFromString(const std::string& string) {
    if (string == "begin")
        return Beam::TYPE_BEGIN;
    if (string == "continue")
        return Beam::TYPE_CONTINUE;
    if (string == "end")
        return Beam::TYPE_END;
    if (string == "forward hook")
        return Beam::TYPE_FORWARD_HOOK;
    if (string == "backward hook")
        return Beam::TYPE_BACKWARD_HOOK;
    return Beam::TYPE_CONTINUE;
}

} // namespace mxml
