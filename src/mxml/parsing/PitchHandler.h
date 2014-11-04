//  Created by Alejandro Isaza on 2/20/2014.
//  Copyright (c) 2014 Venture Media Labs Inc. All rights reserved.

#pragma once
#include <lxml/BaseRecursiveHandler.h>
#include <lxml/DoubleHandler.h>
#include <lxml/IntegerHandler.h>
#include <lxml/StringHandler.h>

#include <mxml/dom/Pitch.h>

namespace mxml {

class PitchHandler : public lxml::BaseRecursiveHandler<dom::Pitch> {
public:
    void startElement(const lxml::QName& qname, const AttributeMap& attributes);
    RecursiveHandler* startSubElement(const lxml::QName& qname);
    void endSubElement(const lxml::QName& qname, lxml::RecursiveHandler* parser);
    
    static dom::Pitch::Step stepFromString(const std::string& string);
    
private:
    lxml::DoubleHandler _doubleHandler;
    lxml::IntegerHandler _integerHandler;
    lxml::StringHandler _stringHandler;
};

} // namespace mxml
