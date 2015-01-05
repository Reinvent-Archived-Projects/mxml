//  Created by Alejandro Isaza on 2014-03-25.
//  Copyright (c) 2014 Venture Media Labs. All rights reserved.

#include "DefaultsHandler.h"

namespace mxml {

using dom::Defaults;
using lxml::QName;

static const char* kScalingTag = "scaling";
static const char* kStaffLayoutTag = "staff-layout";

void DefaultsHandler::startElement(const QName& qname, const AttributeMap& attributes) {
    _result.reset(new Defaults());
}

lxml::RecursiveHandler* DefaultsHandler::startSubElement(const QName& qname) {
    if (strcmp(qname.localName(), kScalingTag) == 0)
        return &_scalingHandler;
    else if (strcmp(qname.localName(), kStaffLayoutTag) == 0)
        return &_staffLayoutHandler;
    return 0;
}

void DefaultsHandler::endSubElement(const QName& qname, RecursiveHandler* parser) {
    if (strcmp(qname.localName(), kScalingTag) == 0)
        _result->setScaling(_scalingHandler.result());
    else if (strcmp(qname.localName(), kStaffLayoutTag) == 0)
        _result->setStaffLayout(_staffLayoutHandler.result());
}

} // namespace mxml
