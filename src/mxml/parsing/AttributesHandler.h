//  Created by Alejandro Isaza on 2/20/2014.
//  Copyright (c) 2014 Venture Media Labs Inc. All rights reserved.

#pragma once
#include <lxml/BaseRecursiveHandler.h>
#include "ClefHandler.h"
#include <lxml/IntegerHandler.h>
#include "KeyHandler.h"
#include "TimeHandler.h"

#include <mxml/dom/Attributes.h>
#include <memory>

namespace mxml {

class AttributesHandler : public lxml::BaseRecursiveHandler<std::unique_ptr<dom::Attributes>> {
public:
    void startElement(const lxml::QName& qname, const AttributeMap& attributes);
    void endElement(const lxml::QName& qname, const std::string& contents);
    
    RecursiveHandler* startSubElement(const lxml::QName& qname);
    void endSubElement(const lxml::QName& qname, RecursiveHandler* parser);
    
    const dom::Attributes& defaultAttributes() const {
        return _defaultAttributes;
    }
    
private:
    dom::Attributes _defaultAttributes;
    ClefHandler _clefHandler;
    lxml::IntegerHandler _integerHandler;
    TimeHandler _timeHandler;
    KeyHandler _keyHandler;
};

} // namespace mxml
