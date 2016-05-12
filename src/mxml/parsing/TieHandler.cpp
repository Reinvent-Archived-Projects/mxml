// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "TieHandler.h"
#include <mxml/dom/InvalidDataError.h>

namespace mxml {

using dom::Tie;
using lxml::QName;

static const char* kTypeAttribute = "type";

void TieHandler::startElement(const QName& qname, const AttributeMap& attributes) {
    _result.reset(new Tie());
    
    auto type = attributes.find(kTypeAttribute);
    if (type != attributes.end())
        _result->setType(typeFromString(type->second));
}

dom::StartStopContinue TieHandler::typeFromString(const std::string& string) {
    if (string == "start")
        return dom::kStart;
    else if (string == "stop")
        return dom::kStop;
    throw dom::InvalidDataError("Invalid tie type " + string);
}

} // namespace mxml
