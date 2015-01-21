//  Copyright (c) 2015 Venture Media Labs. All rights reserved.

#pragma once
#include <lxml/BaseRecursiveHandler.h>
#include <mxml/dom/Defaults.h>
#include <memory>

#include "GenericNodeHandler.h"

namespace mxml {

class AppearanceHandler : public lxml::BaseRecursiveHandler<dom::Appearance> {
public:
    void startElement(const lxml::QName& qname, const AttributeMap& attributes);
    RecursiveHandler* startSubElement(const lxml::QName& qname);
    void endSubElement(const lxml::QName& qname, lxml::RecursiveHandler* parser);

    static dom::LineType lineTypeFromString(const std::string& string);
    static dom::NoteType noteTypeFromString(const std::string& string);
    static dom::DistanceType distanceTypeFromString(const std::string& string);
    
private:
    parsing::GenericNodeHandler _handler;
};

} // namespace mxml
