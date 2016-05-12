// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "IdentificationHandler.h"

namespace mxml {

using dom::TypedValue;
using lxml::QName;

static const char* kTypeAttribute = "type";
static const char* kCreatorTag = "creator";
static const char* kRightsTag = "rights";
static const char* kSourceTag = "source";

void TypedValueHandler::startElement(const QName& qname, const AttributeMap& attributes) {
    _result.reset(new TypedValue());
    
    auto type = attributes.find(kTypeAttribute);
    if (type != attributes.end())
        _result->setType(type->second);
    else
        _result->setType("");
    
    _result->setValue("");
}

void TypedValueHandler::endElement(const QName& qname, const std::string& contents) {
    _result->setValue(contents);
}

lxml::RecursiveHandler* IdentificationHandler::startSubElement(const QName& qname) {
    _result.reset(new dom::Identification());
    
    if (strcmp(qname.localName(), kCreatorTag) == 0)
        return &_typedValueHandler;
    else if (strcmp(qname.localName(), kRightsTag) == 0)
        return &_typedValueHandler;
    else if (strcmp(qname.localName(), kSourceTag) == 0)
        return &_stringHandler;
    return 0;
}

void IdentificationHandler::endSubElement(const QName& qname, RecursiveHandler* parser) {
    if (strcmp(qname.localName(), kCreatorTag) == 0)
        _result->addCreator(_typedValueHandler.result());
    else if (strcmp(qname.localName(), kRightsTag) == 0)
        _result->addRights(_typedValueHandler.result());
    else if (strcmp(qname.localName(), kSourceTag) == 0)
        _result->setSource(_stringHandler.result());
}

} // namespace mxml
