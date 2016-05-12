// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

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
