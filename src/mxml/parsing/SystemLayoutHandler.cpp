// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "SystemLayoutHandler.h"

namespace mxml {

static const char* kSystemMarginsTag = "system-margins";
static const char* kSystemDistanceTag = "system-distance";
static const char* kTopSystemDistanceTag = "top-system-distance";
static const char* kSystemDividersTag = "system-dividers";

void SystemLayoutHandler::startElement(const lxml::QName& qname, const AttributeMap& attributes) {
    _result = dom::SystemLayout{};
}

lxml::RecursiveHandler* SystemLayoutHandler::startSubElement(const lxml::QName& qname) {
    if (strcmp(qname.localName(), kSystemMarginsTag) == 0)
        return &_systemMarginsHandler;
    else if (strcmp(qname.localName(), kSystemDistanceTag) == 0)
        return &_doubleHandler;
    else if (strcmp(qname.localName(), kTopSystemDistanceTag) == 0)
        return &_doubleHandler;
    else if (strcmp(qname.localName(), kSystemDividersTag) == 0)
        return &_systemDividersHandler;
    return 0;
}

void SystemLayoutHandler::endSubElement(const lxml::QName& qname, RecursiveHandler* parser) {
    using dom::presentOptional;

    if (strcmp(qname.localName(), kSystemMarginsTag) == 0) {
        _result.systemMargins = _systemMarginsHandler.result();
    } else if (strcmp(qname.localName(), kSystemDistanceTag) == 0) {
        auto value = static_cast<dom::tenths_t>(_doubleHandler.result());
        _result.systemDistance = presentOptional(value);
    } else if (strcmp(qname.localName(), kTopSystemDistanceTag) == 0) {
        auto value = static_cast<dom::tenths_t>(_doubleHandler.result());
        _result.topSystemDistance = presentOptional(value);
    } else if (strcmp(qname.localName(), kSystemDividersTag) == 0) {
        _result.systemDividers = _systemDividersHandler.result();
    }
}

} // namespace mxml
