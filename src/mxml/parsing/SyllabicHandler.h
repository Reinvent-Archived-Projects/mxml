// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once
#include <lxml/BaseRecursiveHandler.h>
#include <mxml/dom/Syllabic.h>

namespace mxml {

class SyllabicHandler : public lxml::BaseRecursiveHandler<std::unique_ptr<dom::Syllabic>> {
public:
    void startElement(const lxml::QName& qname, const AttributeMap& attributes);
    void endElement(const lxml::QName& qname, const std::string& contents);
    static dom::Syllabic::Type typeFromString(const std::string& string);
};

} // namespace
