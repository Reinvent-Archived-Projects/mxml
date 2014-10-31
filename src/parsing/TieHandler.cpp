//  Created by Alejandro Isaza on 2014-04-21.
//  Copyright (c) 2014 Venture Media Labs. All rights reserved.

#include "TieHandler.h"
#include "dom/InvalidDataError.h"

namespace mxml {

    using lxml::QName;

static const char* kTypeAttribute = "type";

void TieHandler::startElement(const QName& qname, const AttributeMap& attributes) {
    auto type = attributes.find(kTypeAttribute);
    if (type != attributes.end())
        _result.setType(typeFromString(type->second));
}

dom::StartStopContinue TieHandler::typeFromString(const std::string& string) {
    if (string == "start")
        return dom::TYPE_START;
    else if (string == "stop")
        return dom::TYPE_STOP;
    throw dom::InvalidDataError("Invalid tie type " + string);
}

} // namespace mxml
