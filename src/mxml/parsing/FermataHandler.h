// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once
#include <lxml/BaseRecursiveHandler.h>
#include <mxml/dom/Fermata.h>

namespace mxml {

class FermataHandler : public lxml::BaseRecursiveHandler<std::unique_ptr<dom::Fermata>> {
public:
    void startElement(const lxml::QName& qname, const AttributeMap& attributes);
    void endElement(const lxml::QName& qname, const std::string& contents);
    
    static dom::Fermata::Type typeFromString(const std::string& string);
    static dom::Fermata::Shape shapeFromString(const std::string& string);
};

} // namespace mxml
