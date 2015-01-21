//  Created by Alejandro Isaza on 2014-03-25.
//  Copyright (c) 2014 Venture Media Labs. All rights reserved.

#pragma once
#include <lxml/BaseRecursiveHandler.h>
#include <lxml/DoubleHandler.h>

#include <mxml/dom/Scaling.h>

namespace mxml {

class ScalingHandler : public lxml::BaseRecursiveHandler<dom::Scaling> {
public:
    RecursiveHandler* startSubElement(const lxml::QName& qname);
    void endSubElement(const lxml::QName& qname, lxml::RecursiveHandler* parser);
    
private:
    lxml::DoubleHandler _doubleHandler;
};

} // namespace mxml
