// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once
#include <lxml/BaseRecursiveHandler.h>
#include <mxml/dom/Tie.h>

namespace mxml {

class TieHandler : public lxml::BaseRecursiveHandler<std::unique_ptr<dom::Tie>> {
public:
    void startElement(const lxml::QName& qname, const AttributeMap& attributes);
    static dom::StartStopContinue typeFromString(const std::string& string);
};

} // namespace mxml
