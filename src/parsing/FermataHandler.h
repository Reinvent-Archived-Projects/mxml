//  Created by Alejandro Isaza on 2014-04-23.
//  Copyright (c) 2014 Venture Media Labs. All rights reserved.

#pragma once
#include "BaseRecursiveHandler.h"
#include "dom/Fermata.h"

namespace mxml {

class FermataHandler : public lxml::BaseRecursiveHandler<dom::Fermata> {
public:
    void startElement(const lxml::QName& qname, const AttributeMap& attributes);
    void endElement(const lxml::QName& qname, const std::string& contents);
    
    static dom::Fermata::Type typeFromString(const std::string& string);
    static dom::Fermata::Shape shapeFromString(const std::string& string);
};

} // namespace mxml
