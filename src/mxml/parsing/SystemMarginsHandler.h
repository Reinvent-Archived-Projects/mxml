//  Copyright (c) 2015 Venture Media Labs. All rights reserved.

#pragma once
#include <lxml/BaseRecursiveHandler.h>
#include <lxml/DoubleHandler.h>
#include <mxml/dom/SystemLayout.h>
#include <memory>

namespace mxml {

class SystemMarginsHandler : public lxml::BaseRecursiveHandler<dom::SystemMargins> {
public:
    void startElement(const lxml::QName& qname, const AttributeMap& attributes);
    RecursiveHandler* startSubElement(const lxml::QName& qname);
    void endSubElement(const lxml::QName& qname, lxml::RecursiveHandler* parser);
    
private:
    lxml::DoubleHandler _doubleHandler;
};

} // namespace mxml
