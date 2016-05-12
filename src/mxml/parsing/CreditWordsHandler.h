// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once
#include <lxml/BaseRecursiveHandler.h>

#include <mxml/dom/CreditWords.h>

namespace mxml {

class CreditWordsHandler : public lxml::BaseRecursiveHandler<std::unique_ptr<dom::CreditWords>> {
public:
    void startElement(const lxml::QName& qname, const AttributeMap& attributes);
    void endElement(const lxml::QName& qname, const std::string& contents);
    
    static dom::CreditWords::FontStyle fontStyleFromString(const std::string& string);
    static dom::CreditWords::FontWeight fontWeightFromString(const std::string& string);
};

} // namespace mxml
