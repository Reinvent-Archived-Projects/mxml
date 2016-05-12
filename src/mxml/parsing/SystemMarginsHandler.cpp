// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "SystemMarginsHandler.h"

namespace mxml {

static const char* kLeftMarginTag = "left-margin";
static const char* kRightMarginTag = "right-margin";

void SystemMarginsHandler::startElement(const lxml::QName& qname, const AttributeMap& attributes) {
    _result = dom::SystemMargins{};
}

lxml::RecursiveHandler* SystemMarginsHandler::startSubElement(const lxml::QName& qname) {
    if (strcmp(qname.localName(), kLeftMarginTag) == 0 || strcmp(qname.localName(), kRightMarginTag) == 0)
        return &_doubleHandler;
    return 0;
}

void SystemMarginsHandler::endSubElement(const lxml::QName& qname, RecursiveHandler* parser) {
    if (strcmp(qname.localName(), kLeftMarginTag) == 0) {
        auto value = static_cast<dom::tenths_t>(_doubleHandler.result());
        _result.left = dom::presentOptional(value);
    } else if (strcmp(qname.localName(), kRightMarginTag) == 0) {
        auto value = static_cast<dom::tenths_t>(_doubleHandler.result());
        _result.right = dom::presentOptional(value);
    }
}

} // namespace mxml
