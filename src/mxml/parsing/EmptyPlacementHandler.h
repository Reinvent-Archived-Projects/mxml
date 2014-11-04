//  Created by Alejandro Isaza on 2014-04-21.
//  Copyright (c) 2014 Venture Media Labs. All rights reserved.

#pragma once
#include <lxml/BaseRecursiveHandler.h>
#include <mxml/dom/EmptyPlacement.h>

namespace mxml {

class EmptyPlacementHandler : public lxml::BaseRecursiveHandler<dom::EmptyPlacement> {
public:
    void startElement(const lxml::QName& qname, const AttributeMap& attributes);
    static dom::Placement placementFromString(const std::string& string);
};

} // namespace mxml
