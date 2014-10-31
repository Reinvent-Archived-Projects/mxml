//  Created by Alejandro Isaza on 2014-03-25.
//  Copyright (c) 2014 Venture Media Labs. All rights reserved.

#pragma once
#include "BaseRecursiveHandler.h"
#include "StringHandler.h"
#include "dom/Identification.h"

namespace mxml {

class TypedValueHandler : public lxml::BaseRecursiveHandler<dom::TypedValue> {
public:
    void startElement(const lxml::QName& qname, const AttributeMap& attributes);
    void endElement(const lxml::QName& qname, const std::string& contents);
};

class IdentificationHandler : public lxml::BaseRecursiveHandler<dom::Identification> {
public:
    RecursiveHandler* startSubElement(const lxml::QName& qname);
    void endSubElement(const lxml::QName& qname, RecursiveHandler* parser);
    
private:
    TypedValueHandler _typedValueHandler;
    lxml::StringHandler _stringHandler;
};

} // namespace mxml
