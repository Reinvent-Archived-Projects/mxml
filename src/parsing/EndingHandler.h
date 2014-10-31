//  Created by Alejandro Isaza on 2014-05-01.
//  Copyright (c) 2014 Venture Media Labs. All rights reserved.

#pragma once
#include "BaseRecursiveHandler.h"

#include "dom/Ending.h"

namespace mxml {

class EndingHandler : public lxml::BaseRecursiveHandler<dom::Ending> {
public:
    void startElement(const lxml::QName& qname, const AttributeMap& attributes);
    void endElement(const lxml::QName& qname, const std::string& contents);
    
    static dom::Ending::Type typeFromString(const std::string& string);
    static std::set<int> parseNumberList(const std::string& string);
};

} // namespace mxml
