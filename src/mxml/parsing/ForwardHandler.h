//  Created by Alejandro Isaza on 2/25/2014.
//  Copyright (c) 2014 Venture Media Labs Inc. All rights reserved.

#pragma once
#include <lxml/BaseRecursiveHandler.h>
#include <lxml/IntegerHandler.h>

#include <mxml/dom/Forward.h>
#include <memory>

namespace mxml {

class ForwardHandler : public lxml::BaseRecursiveHandler<std::unique_ptr<dom::Forward>> {
public:
    void startElement(const lxml::QName& qname, const AttributeMap& attributes);
    RecursiveHandler* startSubElement(const lxml::QName& qname);
    void endSubElement(const lxml::QName& qname, RecursiveHandler* parser);
    
private:
    lxml::IntegerHandler _integerHandler;
};

} // namespace mxml
