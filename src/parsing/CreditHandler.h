//  Created by Alejandro Isaza on 2014-03-25.
//  Copyright (c) 2014 Venture Media Labs. All rights reserved.

#pragma once
#include "BaseRecursiveHandler.h"
#include "CreditWordsHandler.h"

#include "dom/Credit.h"

namespace mxml {

class CreditHandler : public lxml::BaseRecursiveHandler<dom::Credit> {
public:
    void startElement(const lxml::QName& qname, const AttributeMap& attributes);
    RecursiveHandler* startSubElement(const lxml::QName& qname);
    void endSubElement(const lxml::QName& qname, RecursiveHandler* parser);
    
private:
    CreditWordsHandler _creditWordsHandler;
};

} // namespace mxml
