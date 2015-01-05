//  Created by Alejandro Isaza on 2014-03-25.
//  Copyright (c) 2014 Venture Media Labs. All rights reserved.

#include "StaffLayoutHandler.h"

namespace mxml {

using dom::StaffLayout;
using lxml::QName;

static const char* kNumberAttribute = "number";
static const char* kStaffDistanceTag = "staff-distance";

void StaffLayoutHandler::startElement(const QName& qname, const lxml::RecursiveHandler::AttributeMap& attributes) {
    _result.reset(new StaffLayout());
    
    auto number = attributes.find(kNumberAttribute);
    if (number != attributes.end())
        _result->setNumber(lxml::IntegerHandler::parseInteger(number->second));
}

lxml::RecursiveHandler* StaffLayoutHandler::startSubElement(const QName& qname) {
    if (strcmp(qname.localName(), kStaffDistanceTag) == 0)
        return &_doubleHandler;
    return 0;
}

void StaffLayoutHandler::endSubElement(const QName& qname, RecursiveHandler* parser) {
    using dom::presentOptional;
    
    if (strcmp(qname.localName(), kStaffDistanceTag) == 0)
        _result->setStaffDistance(presentOptional((float)_doubleHandler.result()));
}

} // namespace mxml
