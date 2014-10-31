//  Created by Alejandro Isaza on 2/20/2014.
//  Copyright (c) 2014 Venture Media Labs Inc. All rights reserved.

#pragma once
#include "BaseRecursiveHandler.h"
#include "CreditHandler.h"
#include "DefaultsHandler.h"
#include "IdentificationHandler.h"
#include "PartHandler.h"
#include "dom/Score.h"

#include <memory>

namespace mxml {

class ScoreHandler : public lxml::BaseRecursiveHandler<std::unique_ptr<dom::Score>> {
public:
    void startElement(const lxml::QName& qname, const AttributeMap& attributes);
    RecursiveHandler* startSubElement(const lxml::QName& qname);
    void endSubElement(const lxml::QName& qname, lxml::RecursiveHandler* parser);
    
private:
    IdentificationHandler _identificationHandler;
    CreditHandler _creditHandler;
    DefaultsHandler _defaultsHandler;
    PartHandler _partHandler;
};

} // namespace mxml
