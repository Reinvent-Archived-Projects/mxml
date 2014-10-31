//  Created by Alejandro Isaza on 2014-05-01.
//  Copyright (c) 2014 Venture Media Labs. All rights reserved.

#pragma once
#include "BaseRecursiveHandler.h"
#include "EndingHandler.h"
#include "RepeatHandler.h"
#include "StringHandler.h"

#include "dom/Barline.h"
#include <memory>

namespace mxml {

class BarlineHandler : public lxml::BaseRecursiveHandler<std::unique_ptr<dom::Barline>> {
public:
    void startElement(const lxml::QName& qname, const AttributeMap& attributes);
    RecursiveHandler* startSubElement(const lxml::QName& qname);
    void endSubElement(const lxml::QName& qname, RecursiveHandler* parser);
    
    static dom::Barline::Style styleFromString(const std::string& string);
    static dom::Barline::Location locationFromString(const std::string& string);
    
private:
    lxml::StringHandler _stringHandler;
    EndingHandler _endingHandler;
    RepeatHandler _repeatHandler;
};

} // namespace mxml
