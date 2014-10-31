//  Created by Alejandro Isaza on 2014-04-23.
//  Copyright (c) 2014 Venture Media Labs. All rights reserved.

#pragma once
#include "BaseRecursiveHandler.h"
#include "dom/Tied.h"

namespace mxml {

class TiedHandler : public lxml::BaseRecursiveHandler<dom::Tied> {
public:
    void startElement(const lxml::QName& qname, const AttributeMap& attributes);
    
    static dom::StartStopContinue typeFromString(const std::string& string);
    static dom::Placement placementFromString(const std::string& string);
    static dom::Orientation orientationFromString(const std::string& string);
};

} // namespace mxml
