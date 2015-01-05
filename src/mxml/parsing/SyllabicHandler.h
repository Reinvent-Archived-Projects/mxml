//  Created by Alejandro Isaza on 2014-07-09.
//  Copyright (c) 2014 Venture Media Labs. All rights reserved.

#pragma once
#include <lxml/BaseRecursiveHandler.h>
#include <mxml/dom/Syllabic.h>

namespace mxml {

class SyllabicHandler : public lxml::BaseRecursiveHandler<std::unique_ptr<dom::Syllabic>> {
public:
    void startElement(const lxml::QName& qname, const AttributeMap& attributes);
    void endElement(const lxml::QName& qname, const std::string& contents);
    static dom::Syllabic::Type typeFromString(const std::string& string);
};

} // namespace
