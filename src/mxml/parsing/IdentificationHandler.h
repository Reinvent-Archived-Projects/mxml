// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once
#include <lxml/BaseRecursiveHandler.h>
#include <lxml/StringHandler.h>
#include <mxml/dom/Identification.h>

namespace mxml {

class TypedValueHandler : public lxml::BaseRecursiveHandler<std::unique_ptr<dom::TypedValue>> {
public:
    void startElement(const lxml::QName& qname, const AttributeMap& attributes);
    void endElement(const lxml::QName& qname, const std::string& contents);
};

class IdentificationHandler : public lxml::BaseRecursiveHandler<std::unique_ptr<dom::Identification>> {
public:
    RecursiveHandler* startSubElement(const lxml::QName& qname);
    void endSubElement(const lxml::QName& qname, RecursiveHandler* parser);
    
private:
    TypedValueHandler _typedValueHandler;
    lxml::StringHandler _stringHandler;
};

} // namespace mxml
