//  Copyright (c) 2015 Venture Media Labs. All rights reserved.

#pragma once
#include <lxml/BaseRecursiveHandler.h>
#include <lxml/DoubleHandler.h>

#include <mxml/dom/PageMargins.h>


namespace mxml {

class PageMarginsHandler : public lxml::BaseRecursiveHandler<dom::PageMargins> {
public:
    void startElement(const lxml::QName& qname, const AttributeMap& attributes);
    RecursiveHandler* startSubElement(const lxml::QName& qname);
    void endSubElement(const lxml::QName& qname, lxml::RecursiveHandler* parser);

    static dom::PageMargins::MarginType marginTypeFromString(const std::string& string);

private:
    lxml::DoubleHandler _doubleHandler;
};

} // namespace mxml
