// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once
#include <lxml/BaseRecursiveHandler.h>
#include <mxml/dom/Slur.h>

namespace mxml {

class SlurHandler : public lxml::BaseRecursiveHandler<std::unique_ptr<dom::Slur>> {
public:
    void startElement(const lxml::QName& qname, const AttributeMap& attributes);
    
    static dom::StartStopContinue typeFromString(const std::string& string);
    static dom::Placement placementFromString(const std::string& string);
    static dom::Orientation orientationFromString(const std::string& string);
};

} // namespace mxml
