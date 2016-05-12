// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

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
    
private:
    ClefHandler _clefHandler;
    lxml::IntegerHandler _integerHandler;
    TimeHandler _timeHandler;
    KeyHandler _keyHandler;
};

} // namespace mxml
