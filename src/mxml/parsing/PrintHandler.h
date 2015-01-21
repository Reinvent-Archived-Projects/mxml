//  Copyright (c) 2014 Venture Media Labs. All rights reserved.

#pragma once
#include <lxml/BaseRecursiveHandler.h>
#include <lxml/DoubleHandler.h>

#include <mxml/dom/Print.h>
#include <memory>

#include "FormattedTextHandler.h"
#include "PageLayoutHandler.h"
#include "SystemLayoutHandler.h"
#include "StaffLayoutHandler.h"
#include "GenericNodeHandler.h"

namespace mxml {

class PrintHandler : public lxml::BaseRecursiveHandler<std::unique_ptr<dom::Print>> {
public:
    void startElement(const lxml::QName& qname, const AttributeMap& attributes);
    RecursiveHandler* startSubElement(const lxml::QName& qname);
    void endSubElement(const lxml::QName& qname, lxml::RecursiveHandler* parser);
    
private:
    FormattedTextHandler _formattedTextHandler;
    PageLayoutHandler _pageLayoutHandler;
    SystemLayoutHandler _systemLayoutHandler;
    StaffLayoutHandler _staffLayoutHandler;
    parsing::GenericNodeHandler _genericNodeHandler;
};

} // namespace mxml
