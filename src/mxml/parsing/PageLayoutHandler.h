//  Copyright (c) 2015 Venture Media Labs. All rights reserved.

#pragma once
#include <lxml/BaseRecursiveHandler.h>
#include <lxml/DoubleHandler.h>

#include <mxml/dom/PageLayout.h>

#include "PageMarginsHandler.h"


namespace mxml {

class PageLayoutHandler : public lxml::BaseRecursiveHandler<dom::PageLayout> {
public:
    void startElement(const lxml::QName& qname, const AttributeMap& attributes);
    RecursiveHandler* startSubElement(const lxml::QName& qname);
    void endSubElement(const lxml::QName& qname, lxml::RecursiveHandler* parser);
    
private:
    lxml::DoubleHandler _doubleHandler;
    PageMarginsHandler _pageMarginsHandler;
};

} // namespace mxml
