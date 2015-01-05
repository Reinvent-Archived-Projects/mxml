//  Created by Alejandro Isaza on 2014-03-25.
//  Copyright (c) 2014 Venture Media Labs. All rights reserved.

#pragma once
#include <lxml/BaseRecursiveHandler.h>
#include "ScalingHandler.h"
#include "StafflayoutHandler.h"

#include <mxml/dom/Defaults.h>

namespace mxml {

class DefaultsHandler : public lxml::BaseRecursiveHandler<std::unique_ptr<dom::Defaults>> {
public:
    void startElement(const lxml::QName& qname, const AttributeMap& attributes);
    RecursiveHandler* startSubElement(const lxml::QName& qname);
    void endSubElement(const lxml::QName& qname, RecursiveHandler* parser);
    
private:
    ScalingHandler _scalingHandler;
    StaffLayoutHandler _staffLayoutHandler;
};

} // namespace mxml
