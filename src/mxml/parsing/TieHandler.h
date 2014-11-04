//  Created by Alejandro Isaza on 2014-04-21.
//  Copyright (c) 2014 Venture Media Labs. All rights reserved.

#pragma once
#include <lxml/BaseRecursiveHandler.h>
#include <mxml/dom/Tie.h>

namespace mxml {

class TieHandler : public lxml::BaseRecursiveHandler<dom::Tie> {
public:
    void startElement(const lxml::QName& qname, const AttributeMap& attributes);
    static dom::StartStopContinue typeFromString(const std::string& string);
};

} // namespace mxml
