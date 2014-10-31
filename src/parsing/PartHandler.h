//  Created by Alejandro Isaza on 2/20/2014.
//  Copyright (c) 2014 Venture Media Labs Inc. All rights reserved.

#pragma once
#include "BaseRecursiveHandler.h"
#include "MeasureHandler.h"

#include "dom/Part.h"

#include <memory>

namespace mxml {

class PartHandler : public lxml::BaseRecursiveHandler<std::unique_ptr<dom::Part>> {
public:
    void startElement(const lxml::QName& qname, const AttributeMap& attributes);
    
    RecursiveHandler* startSubElement(const lxml::QName& qname);
    void endSubElement(const lxml::QName& qname, lxml::RecursiveHandler* parser);
    
private:
    MeasureHandler _measureHandler;
};

} // namespace mxml
