//  Copyright (c) 2015 Venture Media Labs. All rights reserved.

#pragma once
#include <lxml/BaseRecursiveHandler.h>
#include <lxml/DoubleHandler.h>

#include <mxml/dom/SystemLayout.h>

#include "SystemDividersHandler.h"
#include "SystemMarginsHandler.h"


namespace mxml {

class SystemLayoutHandler : public lxml::BaseRecursiveHandler<dom::SystemLayout> {
public:
    void startElement(const lxml::QName& qname, const AttributeMap& attributes);
    RecursiveHandler* startSubElement(const lxml::QName& qname);
    void endSubElement(const lxml::QName& qname, RecursiveHandler* handler);

private:
    lxml::DoubleHandler _doubleHandler;
    SystemDividersHandler _systemDividersHandler;
    SystemMarginsHandler _systemMarginsHandler;
};

} // namespace mxml
