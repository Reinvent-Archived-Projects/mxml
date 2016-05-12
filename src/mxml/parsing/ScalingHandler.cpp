// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "ScalingHandler.h"

namespace mxml {

using dom::Scaling;
using lxml::QName;

static const char* kMillimetersTag = "millimeters";
static const char* kTenthsTag = "tenths";

lxml::RecursiveHandler* ScalingHandler::startSubElement(const QName& qname) {
    _result = Scaling{};
    
    if (strcmp(qname.localName(), kMillimetersTag) == 0)
        return &_doubleHandler;
    else if (strcmp(qname.localName(), kTenthsTag) == 0)
        return &_doubleHandler;
    return 0;
}

void ScalingHandler::endSubElement(const QName& qname, RecursiveHandler* parser) {
    if (strcmp(qname.localName(), kMillimetersTag) == 0)
        _result.millimeters = (float)_doubleHandler.result();
    else if (strcmp(qname.localName(), kTenthsTag) == 0)
        _result.tenths = (float)_doubleHandler.result();
}

} // namespace mxml
