// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "PageMarginsHandler.h"

#include <mxml/dom/InvalidDataError.h>

namespace mxml {

static const char* kTypeAttribute = "type";
static const char* kLeftMarginTag = "left-margin";
static const char* kRightMarginTag = "right-margin";
static const char* kTopMarginTag = "top-margin";
static const char* kBottomMarginTag = "bottom-margin";

void PageMarginsHandler::startElement(const lxml::QName& qname, const AttributeMap& attributes) {
    _result = dom::PageMargins{};

    auto it = attributes.find(kTypeAttribute);
    if (it != attributes.end())
        _result.type = marginTypeFromString(it->second);
}

lxml::RecursiveHandler* PageMarginsHandler::startSubElement(const lxml::QName& qname) {
    if (strcmp(qname.localName(), kLeftMarginTag) == 0)
        return &_doubleHandler;
    else  if (strcmp(qname.localName(), kRightMarginTag) == 0)
        return &_doubleHandler;
    else  if (strcmp(qname.localName(), kTopMarginTag) == 0)
        return &_doubleHandler;
    else  if (strcmp(qname.localName(), kBottomMarginTag) == 0)
        return &_doubleHandler;
    return 0;
}

void PageMarginsHandler::endSubElement(const lxml::QName& qname, RecursiveHandler* parser) {
    if (strcmp(qname.localName(), kLeftMarginTag) == 0) {
        auto value = static_cast<dom::tenths_t>(_doubleHandler.result());
        _result.left = dom::presentOptional(value);
    } else if (strcmp(qname.localName(), kRightMarginTag) == 0) {
        auto value = static_cast<dom::tenths_t>(_doubleHandler.result());
        _result.right = dom::presentOptional(value);
    } else if (strcmp(qname.localName(), kTopMarginTag) == 0) {
        auto value = static_cast<dom::tenths_t>(_doubleHandler.result());
        _result.top = dom::presentOptional(value);
    } else if (strcmp(qname.localName(), kBottomMarginTag) == 0) {
        auto value = static_cast<dom::tenths_t>(_doubleHandler.result());
        _result.bottom = dom::presentOptional(value);
    }
}

dom::PageMargins::MarginType PageMarginsHandler::marginTypeFromString(const std::string& string) {
    if (string == "odd")
        return dom::PageMargins::MarginType::Odd;
    if (string == "even")
        return dom::PageMargins::MarginType::Even;
    if (string == "both")
        return dom::PageMargins::MarginType::Both;

    throw dom::InvalidDataError("Invalid margin type " + string);
}
} // namespace mxml
