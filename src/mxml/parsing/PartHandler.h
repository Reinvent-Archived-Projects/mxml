// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once
#include <lxml/BaseRecursiveHandler.h>
#include "MeasureHandler.h"

#include <mxml/dom/Part.h>

#include <memory>

namespace mxml {
namespace parsing {

class PartHandler : public lxml::BaseRecursiveHandler<std::unique_ptr<dom::Part>> {
public:
    void startElement(const lxml::QName& qname, const AttributeMap& attributes);
    
    RecursiveHandler* startSubElement(const lxml::QName& qname);
    void endSubElement(const lxml::QName& qname, lxml::RecursiveHandler* parser);
    
private:
    MeasureHandler _measureHandler;
    std::size_t _measureIndex;
};

} // namespace parsing
} // namespace mxml
