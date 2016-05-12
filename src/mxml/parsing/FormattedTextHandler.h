// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once
#include <lxml/BaseRecursiveHandler.h>
#include <mxml/dom/FormattedText.h>

#include "GenericNode.h"


namespace mxml {

class FormattedTextHandler : public lxml::BaseRecursiveHandler<dom::FormattedText> {
public:
    void startElement(const lxml::QName& qname, const AttributeMap& attributes);
    void endElement(const lxml::QName& qname, const std::string& contents);

    static dom::FormattedText buildFromGenericNode(const parsing::GenericNode& node);
};

} // namespace mxml
