//  Created by Alejandro Isaza on 2/25/2014.
//  Copyright (c) 2014 Venture Media Labs Inc. All rights reserved.

#include "ForwardHandler.h"

namespace mxml {

using dom::Forward;
using lxml::QName;

static const char* kDurationTag = "duration";

void ForwardHandler::startElement(const QName& qname, const AttributeMap& attributes) {
    _result.reset(new Forward());
}

lxml::RecursiveHandler* ForwardHandler::startSubElement(const QName& qname) {
    if (strcmp(qname.localName(), kDurationTag) == 0)
        return &_integerHandler;
    return 0;
}

void ForwardHandler::endSubElement(const QName& qname, RecursiveHandler* parser) {
    if (strcmp(qname.localName(), kDurationTag) == 0)
        _result->setDuration(_integerHandler.result());
}

} // namespace mxml
