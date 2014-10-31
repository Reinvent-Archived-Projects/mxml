//  Created by Alejandro Isaza on 2014-03-20.
//  Copyright (c) 2014 Venture Media Labs. All rights reserved.

#pragma once
#include "BaseRecursiveHandler.h"
#include "DoubleHandler.h"
#include "IntegerHandler.h"

#include "dom/Direction.h"
#include "DirectionTypeHandler.h"
#include "SoundHandler.h"

#include <memory>

namespace mxml {

class DirectionHandler : public lxml::BaseRecursiveHandler<std::unique_ptr<dom::Direction>> {
public:
    void startElement(const lxml::QName& qname, const AttributeMap& attributes);
    
    RecursiveHandler* startSubElement(const lxml::QName& qname);
    void endSubElement(const lxml::QName& qname, RecursiveHandler* parser);
    
    static dom::Placement placementFromString(const std::string& string);
    
private:
    lxml::DoubleHandler _doubleHandler;
    lxml::IntegerHandler _integerHandler;
    DirectionTypeHandler _directionTypeHandler;
    SoundHandler _soundHandler;
};

} // namespace mxml
