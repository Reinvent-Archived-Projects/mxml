//  Created by Alejandro Isaza on 2014-05-13.
//  Copyright (c) 2014 Venture Media Labs. All rights reserved.

#pragma once
#include "BaseRecursiveHandler.h"
#include "IntegerHandler.h"
#include "StringHandler.h"

#include "dom/Key.h"

namespace mxml {

class KeyHandler : public lxml::BaseRecursiveHandler<dom::Key> {
public:
    void startElement(const lxml::QName& qname, const AttributeMap& attributes);
    RecursiveHandler* startSubElement(const lxml::QName& qname);
    void endSubElement(const lxml::QName& qname, RecursiveHandler* parser);
    
    static dom::Key::Mode modeFromString(const std::string& string);
    
private:
    lxml::IntegerHandler _integerHandler;
    lxml::StringHandler _stringHandler;
};

} // namespace mxml
