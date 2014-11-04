//  Created by Alejandro Isaza on 2014-03-25.
//  Copyright (c) 2014 Venture Media Labs. All rights reserved.

#include "PrintHandler.h"
#include <lxml/DoubleHandler.h>

namespace mxml {

using dom::Print;
using lxml::QName;

static const char* kStaffSpacingAttribute = "staff-spacing";
static const char* kStaffLayoutAttribute = "staff-layout";

void PrintHandler::startElement(const QName& qname, const AttributeMap& attributes) {
    using dom::presentOptional;

    _result.reset(new Print);
    
    auto staffSpacing = attributes.find(kStaffSpacingAttribute);
    if (staffSpacing != attributes.end())
        _result->setStaffSpacing(presentOptional((float)lxml::DoubleHandler::parseDouble(staffSpacing->second)));
}

lxml::RecursiveHandler* PrintHandler::startSubElement(const QName& qname) {
    if (strcmp(qname.localName(), kStaffLayoutAttribute) == 0)
        return &_staffLayoutHandler;
    return 0;

}

void PrintHandler::endSubElement(const QName& qname, RecursiveHandler* parser) {
    if (strcmp(qname.localName(), kStaffLayoutAttribute) == 0)
        _result->setStaffLayout(presentOptional(_staffLayoutHandler.result()));
}

} // namespace mxml
