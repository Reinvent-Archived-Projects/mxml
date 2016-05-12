// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

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
