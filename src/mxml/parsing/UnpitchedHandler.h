// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once
#include <lxml/BaseRecursiveHandler.h>
#include <lxml/IntegerHandler.h>
#include <lxml/StringHandler.h>

#include <mxml/dom/Unpitched.h>

namespace mxml {

class UnpitchedHandler : public lxml::BaseRecursiveHandler<std::unique_ptr<dom::Unpitched>> {
public:
    void startElement(const lxml::QName& qname, const AttributeMap& attributes);
    virtual RecursiveHandler* startSubElement(const lxml::QName& qname);
    virtual void endSubElement(const lxml::QName& qname, lxml::RecursiveHandler* parser);
    
private:
    lxml::IntegerHandler _integerHandler;
    lxml::StringHandler _stringHandler;
};

} // namespace mxml
