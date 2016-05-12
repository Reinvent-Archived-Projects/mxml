// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "DefaultsHandler.h"
#include <cstring>


namespace mxml {

static const char* kScalingTag = "scaling";
static const char* kPageLayoutTag = "page-layout";
static const char* kSystemLayoutTag = "system-layout";
static const char* kStaffLayoutTag = "staff-layout";
static const char* kAppearanceTag = "appearance";

void DefaultsHandler::startElement(const lxml::QName& qname, const AttributeMap& attributes) {
    _result.reset(new dom::Defaults());
}

lxml::RecursiveHandler* DefaultsHandler::startSubElement(const lxml::QName& qname) {
    if (strcmp(qname.localName(), kScalingTag) == 0)
        return &_scalingHandler;
    else if (strcmp(qname.localName(), kPageLayoutTag) == 0)
        return &_pageLayoutHandler;
    else if (strcmp(qname.localName(), kSystemLayoutTag) == 0)
        return &_systemLayoutHandler;
    else if (strcmp(qname.localName(), kStaffLayoutTag) == 0)
        return &_staffLayoutHandler;
    else if (strcmp(qname.localName(), kAppearanceTag) == 0)
        return &_appearanceHandler;
    return 0;
}

void DefaultsHandler::endSubElement(const lxml::QName& qname, RecursiveHandler* parser) {
    if (strcmp(qname.localName(), kScalingTag) == 0) {
        _result->scaling = dom::presentOptional(_scalingHandler.result());
    } else if (strcmp(qname.localName(), kPageLayoutTag) == 0) {
        _result->pageLayout = dom::presentOptional(_pageLayoutHandler.result());
    } else if (strcmp(qname.localName(), kSystemLayoutTag) == 0) {
        _result->systemLayout = dom::presentOptional(_systemLayoutHandler.result());
    } else if (strcmp(qname.localName(), kStaffLayoutTag) == 0) {
        auto staffLayout = _staffLayoutHandler.result();
        _result->staffDistances[staffLayout.number] = staffLayout.staffDistance;
    } else if (strcmp(qname.localName(), kAppearanceTag) == 0) {
        _result->appearance = _appearanceHandler.result();
    }
}

} // namespace mxml
