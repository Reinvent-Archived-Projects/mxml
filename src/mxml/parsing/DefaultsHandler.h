//  Created by Alejandro Isaza on 2014-03-25.
//  Copyright (c) 2014 Venture Media Labs. All rights reserved.

#pragma once
#include <memory>

#include <lxml/BaseRecursiveHandler.h>
#include <mxml/dom/Defaults.h>

#include "AppearanceHandler.h"
#include "PageLayoutHandler.h"
#include "ScalingHandler.h"
#include "StaffLayoutHandler.h"
#include "SystemLayoutHandler.h"


namespace mxml {

class DefaultsHandler : public lxml::BaseRecursiveHandler<std::unique_ptr<dom::Defaults>> {
public:
    void startElement(const lxml::QName& qname, const AttributeMap& attributes);
    RecursiveHandler* startSubElement(const lxml::QName& qname);
    void endSubElement(const lxml::QName& qname, RecursiveHandler* parser);

private:
    AppearanceHandler _appearanceHandler;
    PageLayoutHandler _pageLayoutHandler;
    ScalingHandler _scalingHandler;
    StaffLayoutHandler _staffLayoutHandler;
    SystemLayoutHandler _systemLayoutHandler;
};

} // namespace mxml
