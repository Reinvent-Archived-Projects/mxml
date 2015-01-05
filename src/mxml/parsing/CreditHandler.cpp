//  Created by Alejandro Isaza on 2014-03-25.
//  Copyright (c) 2014 Venture Media Labs. All rights reserved.

#include "CreditHandler.h"
#include <lxml/IntegerHandler.h>

namespace mxml {

using dom::Credit;
using lxml::QName;

static const char* kPageAttribute = "page";
static const char* kCreditWordsTag = "credit-words";

void CreditHandler::startElement(const QName& qname, const AttributeMap& attributes) {
    _result.reset(new Credit());
    
    auto page = attributes.find(kPageAttribute);
    if (page != attributes.end())
        _result->setPage(lxml::IntegerHandler::parseInteger(page->second));
}

lxml::RecursiveHandler* CreditHandler::startSubElement(const QName& qname) {
    if (strcmp(qname.localName(), kCreditWordsTag) == 0)
        return &_creditWordsHandler;
    return 0;
}

void CreditHandler::endSubElement(const QName& qname, RecursiveHandler* parser) {
    if (strcmp(qname.localName(), kCreditWordsTag) == 0)
        _result->addCreditWords(_creditWordsHandler.result());
}

} // namespace mxml
