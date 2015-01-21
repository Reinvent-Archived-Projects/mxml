//  Copyright (c) 2015 Venture Media Labs. All rights reserved.

#pragma once
#include <lxml/BaseRecursiveHandler.h>
#include <mxml/dom/SystemLayout.h>

namespace mxml {

class SystemDividerHandler : public lxml::BaseRecursiveHandler<dom::SystemDivider> {
public:
    void startElement(const lxml::QName& qname, const AttributeMap& attributes);
};

class SystemDividersHandler : public lxml::BaseRecursiveHandler<dom::SystemDividers> {
public:
    void startElement(const lxml::QName& qname, const AttributeMap& attributes);
    RecursiveHandler* startSubElement(const lxml::QName& qname);
    void endSubElement(const lxml::QName& qname, lxml::RecursiveHandler* parser);
    
private:
    SystemDividerHandler _handler;
};

} // namespace mxml
