//  Created by Alejandro Isaza on 2014-03-25.
//  Copyright (c) 2014 Venture Media Labs. All rights reserved.

#pragma once
#include <lxml/BaseRecursiveHandler.h>

#include <mxml/dom/CreditWords.h>

namespace mxml {

class CreditWordsHandler : public lxml::BaseRecursiveHandler<std::unique_ptr<dom::CreditWords>> {
public:
    void startElement(const lxml::QName& qname, const AttributeMap& attributes);
    void endElement(const lxml::QName& qname, const std::string& contents);
    
    static dom::CreditWords::Justify justifyFromString(const std::string& string);
    static dom::CreditWords::FontStyle fontStyleFromString(const std::string& string);
    static dom::CreditWords::FontWeight fontWeightFromString(const std::string& string);
};

} // namespace mxml
