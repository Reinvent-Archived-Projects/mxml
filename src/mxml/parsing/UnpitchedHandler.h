//  Created by Alejandro Isaza on 2/20/2014.
//  Copyright (c) 2014 Venture Media Labs Inc. All rights reserved.

#pragma once
#include <lxml/BaseRecursiveHandler.h>
#include <lxml/IntegerHandler.h>
#include <lxml/StringHandler.h>

#include <mxml/dom/Unpitched.h>

namespace mxml {

class UnpitchedHandler : public lxml::BaseRecursiveHandler<std::unique_ptr<dom::Unpitched>> {
public:
    void startElement(const lxml::QName& qname, const AttributeMap& attributes);
    virtual RecursiveHandler* startSubElement(const lxml::QName& qname);
    virtual void endSubElement(const lxml::QName& qname, lxml::RecursiveHandler* parser);
    
private:
    lxml::IntegerHandler _integerHandler;
    lxml::StringHandler _stringHandler;
};

} // namespace mxml
