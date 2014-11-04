//  Created by Alejandro Isaza on 2014-05-01.
//  Copyright (c) 2014 Venture Media Labs. All rights reserved.

#pragma once
#include <lxml/BaseRecursiveHandler.h>

#include <mxml/dom/Repeat.h>

namespace mxml {

class RepeatHandler : public lxml::BaseRecursiveHandler<dom::Repeat> {
public:
    void startElement(const lxml::QName& qname, const AttributeMap& attributes);
    
    static dom::Repeat::Direction directionFromString(const std::string& string);
};

} // namespace mxml
