// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "PageLayoutHandler.h"

namespace mxml {

static const char* kPageHeightTag = "page-height";
static const char* kPageWidthTag = "page-width";
static const char* kPageMarginsTag = "page-margins";

void PageLayoutHandler::startElement(const lxml::QName& qname, const AttributeMap& attributes) {
    _result = dom::PageLayout{};
}

lxml::RecursiveHandler* PageLayoutHandler::startSubElement(const lxml::QName& qname) {
    if (strcmp(qname.localName(), kPageHeightTag) == 0)
        return &_doubleHandler;
    else if (strcmp(qname.localName(), kPageWidthTag) == 0)
        return &_doubleHandler;
    else if (strcmp(qname.localName(), kPageMarginsTag) == 0)
        return &_pageMarginsHandler;
    return 0;
}

void PageLayoutHandler::endSubElement(const lxml::QName& qname, RecursiveHandler* parser) {
    if (strcmp(qname.localName(), kPageHeightTag) == 0) {
        auto value = static_cast<dom::tenths_t>(_doubleHandler.result());
        _result.pageHeight = dom::presentOptional(value);
    } else if (strcmp(qname.localName(), kPageWidthTag) == 0) {
        auto value = static_cast<dom::tenths_t>(_doubleHandler.result());
        _result.pageWidth = dom::presentOptional(value);
    } else if (strcmp(qname.localName(), kPageMarginsTag) == 0) {
        auto margins = _pageMarginsHandler.result();
        if (margins.type == dom::PageMargins::MarginType::Odd || margins.type == dom::PageMargins::MarginType::Both)
            _result.oddPageMargins = margins;
        if (margins.type == dom::PageMargins::MarginType::Even || margins.type == dom::PageMargins::MarginType::Both)
            _result.evenPageMargins = margins;
    }
}

} // namespace mxml
