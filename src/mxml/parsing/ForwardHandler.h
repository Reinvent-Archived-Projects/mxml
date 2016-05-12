// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once
#include <lxml/BaseRecursiveHandler.h>
#include <lxml/IntegerHandler.h>

#include <mxml/dom/Forward.h>
#include <memory>

namespace mxml {

class ForwardHandler : public lxml::BaseRecursiveHandler<std::unique_ptr<dom::Forward>> {
public:
    void startElement(const lxml::QName& qname, const AttributeMap& attributes);
    RecursiveHandler* startSubElement(const lxml::QName& qname);
    void endSubElement(const lxml::QName& qname, RecursiveHandler* parser);
    
private:
    lxml::IntegerHandler _integerHandler;
};

} // namespace mxml
