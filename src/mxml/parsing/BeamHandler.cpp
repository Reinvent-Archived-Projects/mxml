//  Created by Alejandro Isaza on 2014-03-24.
//  Copyright (c) 2014 Venture Media Labs. All rights reserved.

#include "BeamHandler.h"
#include <lxml/IntegerHandler.h>

namespace mxml {

using dom::Beam;
using lxml::QName;

static const char* kNumberAttribute = "number";

void BeamHandler::startElement(const QName& qname, const AttributeMap& attributes) {
    _result.reset(new Beam());
    
    auto number = attributes.find(kNumberAttribute);
    if (number != attributes.end())
        _result->setNumber(lxml::IntegerHandler::parseInteger(number->second));
}

void BeamHandler::endElement(const QName& qname, const std::string& contents) {
    _result->setType(typeFromString(contents));
}

Beam::Type BeamHandler::typeFromString(const std::string& string) {
    if (string == "begin")
        return Beam::Type::Begin;
    if (string == "continue")
        return Beam::Type::Continue;
    if (string == "end")
        return Beam::Type::End;
    if (string == "forward hook")
        return Beam::Type::ForwardHook;
    if (string == "backward hook")
        return Beam::Type::BackwardHook;
    return Beam::Type::Continue;
}

} // namespace mxml
