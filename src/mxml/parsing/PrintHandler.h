//  Created by Alejandro Isaza on 2014-03-25.
//  Copyright (c) 2014 Venture Media Labs. All rights reserved.

#pragma once
#include <lxml/BaseRecursiveHandler.h>
#include "StaffLayoutHandler.h"

#include <mxml/dom/Print.h>
#include <memory>

namespace mxml {

class PrintHandler : public lxml::BaseRecursiveHandler<std::unique_ptr<dom::Print>> {
public:
    void startElement(const lxml::QName& qname, const AttributeMap& attributes);
    RecursiveHandler* startSubElement(const lxml::QName& qname);
    void endSubElement(const lxml::QName& qname, lxml::RecursiveHandler* parser);
    
private:
    StaffLayoutHandler _staffLayoutHandler;
};

} // namespace mxml
