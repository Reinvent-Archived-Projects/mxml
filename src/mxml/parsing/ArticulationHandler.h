//  Created by Alejandro Isaza on 2014-04-23.
//  Copyright (c) 2014 Venture Media Labs. All rights reserved.

#pragma once
#include <lxml/BaseRecursiveHandler.h>
#include "EmptyPlacementHandler.h"
#include <mxml/dom/Articulation.h>

#include <string>

namespace mxml {

class ArticulationHandler : public lxml::BaseRecursiveHandler<std::unique_ptr<dom::Articulation>> {
public:
    static const std::string kTagNames[];
    
public:
    void startElement(const lxml::QName& qname, const lxml::RecursiveHandler::AttributeMap& attributes);
    
    static dom::Articulation::Type typeFromString(const std::string& string);
    
private:
    EmptyPlacementHandler _emptyPlacementHandler;
};

} // namespace mxml
