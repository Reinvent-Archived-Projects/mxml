// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once
#include <lxml/BaseRecursiveHandler.h>

#include <mxml/dom/Repeat.h>

namespace mxml {

class RepeatHandler : public lxml::BaseRecursiveHandler<std::unique_ptr<dom::Repeat>> {
public:
    void startElement(const lxml::QName& qname, const AttributeMap& attributes);
    
    static dom::Repeat::Direction directionFromString(const std::string& string);
};

} // namespace mxml
