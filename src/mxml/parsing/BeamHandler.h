//  Created by Alejandro Isaza on 2014-03-24.
//  Copyright (c) 2014 Venture Media Labs. All rights reserved.

#pragma once
#include <lxml/BaseRecursiveHandler.h>
#include <mxml/dom/Beam.h>

namespace mxml {

class BeamHandler : public lxml::BaseRecursiveHandler<dom::Beam> {
public:
    void startElement(const lxml::QName& qname, const AttributeMap& attributes);
    void endElement(const lxml::QName& qname, const std::string& contents);
    
    static dom::Beam::Type typeFromString(const std::string& string);
};

} // namespace mxml
