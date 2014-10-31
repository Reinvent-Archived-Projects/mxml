//  Created by Alejandro Isaza on 2/20/2014.
//  Copyright (c) 2014 Venture Media Labs Inc. All rights reserved.

#pragma once
#include "BaseRecursiveHandler.h"
#include "IntegerHandler.h"
#include "StringHandler.h"

#include "dom/Time.h"

namespace mxml {

class TimeHandler : public lxml::BaseRecursiveHandler<dom::Time> {
public:
    void startElement(const lxml::QName& qname, const AttributeMap& attributes);
    RecursiveHandler* startSubElement(const lxml::QName& qname);
    void endSubElement(const lxml::QName& qname, lxml::RecursiveHandler* parser);
    
    static dom::Time::Symbol symbolFromString(const std::string& string);
    
private:
    lxml::IntegerHandler _integerHandler;
    lxml::StringHandler _stringHandler;
};

} // namespace mxml
