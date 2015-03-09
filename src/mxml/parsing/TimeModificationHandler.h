//  Copyright (c) 2015 Venture Media Labs. All rights reserved.

#pragma once
#include <lxml/BaseRecursiveHandler.h>
#include <lxml/IntegerHandler.h>
#include <mxml/dom/TimeModification.h>
#include <memory>

namespace mxml {

class TimeModificationHandler : public lxml::BaseRecursiveHandler<std::unique_ptr<dom::TimeModification>> {
public:
    void startElement(const lxml::QName& qname, const AttributeMap& attributes);
    RecursiveHandler* startSubElement(const lxml::QName& qname);
    void endSubElement(const lxml::QName& qname, lxml::RecursiveHandler* parser);
    
private:
    lxml::IntegerHandler _integerHandler;
};

} // namespace mxml
