// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once
#include <lxml/BaseRecursiveHandler.h>
#include <lxml/DoubleHandler.h>
#include <lxml/IntegerHandler.h>

#include <mxml/dom/Direction.h>
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
