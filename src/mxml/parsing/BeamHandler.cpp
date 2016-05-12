// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

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
