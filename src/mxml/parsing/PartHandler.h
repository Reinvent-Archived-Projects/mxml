//  Created by Alejandro Isaza on 2/20/2014.
//  Copyright (c) 2014 Venture Media Labs Inc. All rights reserved.

#pragma once
#include <lxml/BaseRecursiveHandler.h>
#include "MeasureHandler.h"

#include <mxml/dom/Part.h>

#include <memory>

namespace mxml {
namespace parsing {

class PartHandler : public lxml::BaseRecursiveHandler<std::unique_ptr<dom::Part>> {
public:
    void startElement(const lxml::QName& qname, const AttributeMap& attributes);
    
    RecursiveHandler* startSubElement(const lxml::QName& qname);
    void endSubElement(const lxml::QName& qname, lxml::RecursiveHandler* parser);
    
private:
    MeasureHandler _measureHandler;
    std::size_t _measureIndex;
};

} // namespace parsing
} // namespace mxml
