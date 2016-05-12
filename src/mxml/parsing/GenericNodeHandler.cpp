// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "GenericNodeHandler.h"

#include <lxml/StringHandler.h>


namespace mxml {
namespace parsing {

void GenericNodeHandler::startElement(const lxml::QName& qname, const AttributeMap& attributes) {
    _result.reset(new GenericNode{qname.localName()});
    for (auto pair : attributes) {
        _result->setAttribute(pair.first.localName(), pair.second);
    }
}

void GenericNodeHandler::endElement(const lxml::QName& qname, const std::string& contents) {
    _result->setText(lxml::StringHandler::trim(contents));
}

lxml::RecursiveHandler* GenericNodeHandler::startSubElement(const lxml::QName& qname) {
    if (!_subHandler)
        _subHandler.reset(new GenericNodeHandler{});
    return _subHandler.get();
}

void GenericNodeHandler::endSubElement(const lxml::QName& qname, RecursiveHandler* parser) {
    _result->addChild(_subHandler->result());
}

} // namespace parsing
} // namespace mxml
