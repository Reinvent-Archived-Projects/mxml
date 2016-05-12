// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "SystemDividersHandler.h"
#include "PositionFactory.h"

namespace mxml {

using namespace parsing;

static const char* kLeftDividerTag = "left-divider";
static const char* kRightDividerTag = "right-divider";

void SystemDividerHandler::startElement(const lxml::QName& qname, const AttributeMap& attributes) {
    _result = dom::SystemDivider{};

    _result.position = PositionFactory::buildFromAttributes(attributes);
    _result.horizontalAlignment = HAlignFactory::buildFromAttributes(attributes);
    _result.verticalAlignment = VAlignFactory::buildFromAttributes(attributes);
    _result.printObject = PrintObjectFactory::buildFromAttributes(attributes);
}

void SystemDividersHandler::startElement(const lxml::QName& qname, const AttributeMap& attributes) {
    _result = dom::SystemDividers{};
}

lxml::RecursiveHandler* SystemDividersHandler::startSubElement(const lxml::QName& qname) {
    if (strcmp(qname.localName(), kLeftDividerTag) == 0 || strcmp(qname.localName(), kRightDividerTag))
        return &_handler;
    return 0;
}

void SystemDividersHandler::endSubElement(const lxml::QName& qname, RecursiveHandler* parser) {
    if (strcmp(qname.localName(), kLeftDividerTag) == 0) {
        _result.leftDivider = _handler.result();
    } else if (strcmp(qname.localName(), kRightDividerTag) == 0) {
        _result.rightDivider = _handler.result();
    }
}

} // namespace mxml
