// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once
#include <lxml/BaseRecursiveHandler.h>
#include <lxml/IntegerHandler.h>
#include <lxml/StringHandler.h>
#include <mxml/dom/Tuplet.h>
#include <memory>

#include "GenericNodeHandler.h"


namespace mxml {
namespace parsing {

class TupletHandler : public lxml::BaseRecursiveHandler<std::unique_ptr<dom::Tuplet>> {
public:
    void startElement(const lxml::QName& qname, const AttributeMap& attributes);
    RecursiveHandler* startSubElement(const lxml::QName& qname);
    void endSubElement(const lxml::QName& qname, lxml::RecursiveHandler* parser);

    static dom::Tuplet::Type typeFromString(const std::string& string);
    static dom::Tuplet::Show showFromString(const std::string& string);
    
private:
    lxml::IntegerHandler _integerHandler;
    lxml::StringHandler _stringHandler;
    GenericNodeHandler _genericNodeHandler;
};

} // namespace parsing
} // namespace mxml
