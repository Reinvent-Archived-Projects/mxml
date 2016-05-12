// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "StaffLayoutHandler.h"
#include <lxml/IntegerHandler.h>

namespace mxml {

static const char* kNumberAttribute = "number";
static const char* kStaffDistanceTag = "staff-distance";

void StaffLayoutHandler::startElement(const lxml::QName& qname, const lxml::RecursiveHandler::AttributeMap& attributes) {
    _result = StaffLayout{};
    
    auto number = attributes.find(kNumberAttribute);
    if (number != attributes.end())
        _result.number = lxml::IntegerHandler::parseInteger(number->second);
}

lxml::RecursiveHandler* StaffLayoutHandler::startSubElement(const lxml::QName& qname) {
    if (strcmp(qname.localName(), kStaffDistanceTag) == 0)
        return &_doubleHandler;
    return 0;
}

void StaffLayoutHandler::endSubElement(const lxml::QName& qname, RecursiveHandler* parser) {
    if (strcmp(qname.localName(), kStaffDistanceTag) == 0) {
        auto value = static_cast<dom::tenths_t>(_doubleHandler.result());
        _result.staffDistance = value;
    }
}

} // namespace mxml
