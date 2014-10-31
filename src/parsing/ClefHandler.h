//  Created by Alejandro Isaza on 2/20/2014.
//  Copyright (c) 2014 Venture Media Labs Inc. All rights reserved.

#pragma once
#include "BaseRecursiveHandler.h"
#include "IntegerHandler.h"
#include "StringHandler.h"

#include "dom/Clef.h"

namespace mxml {

class ClefHandler : public lxml::BaseRecursiveHandler<dom::Clef> {
public:
    RecursiveHandler* startSubElement(const lxml::QName& qname);
    void endSubElement(const lxml::QName& qname, RecursiveHandler* parser);
    void startElement(const lxml::QName& qname, const AttributeMap& attributes);
    
    static dom::Clef::Sign signFromString(const std::string& string);
    
private:
    lxml::IntegerHandler _integerHandler;
    lxml::StringHandler _stringHandler;
};

} // namespace mxml
