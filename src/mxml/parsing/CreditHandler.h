// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once
#include <lxml/BaseRecursiveHandler.h>
#include "CreditWordsHandler.h"

#include <mxml/dom/Credit.h>

namespace mxml {

class CreditHandler : public lxml::BaseRecursiveHandler<std::unique_ptr<dom::Credit>> {
public:
    void startElement(const lxml::QName& qname, const AttributeMap& attributes);
    RecursiveHandler* startSubElement(const lxml::QName& qname);
    void endSubElement(const lxml::QName& qname, RecursiveHandler* parser);
    
private:
    CreditWordsHandler _creditWordsHandler;
};

} // namespace mxml
