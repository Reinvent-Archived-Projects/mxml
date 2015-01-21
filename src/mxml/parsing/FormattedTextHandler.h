//  Copyright (c) 2015 Venture Media Labs. All rights reserved.

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
