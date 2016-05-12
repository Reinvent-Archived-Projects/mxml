// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "TiedHandler.h"
#include <lxml/IntegerHandler.h>
#include <mxml/dom/InvalidDataError.h>

namespace mxml {

using dom::Tied;
using lxml::QName;

static const char* kNumberAttribute = "number";
static const char* kTypeAttribute = "type";
static const char* kPlacementAttribute = "placement";
static const char* kOrientationAttribute = "orientation";

void TiedHandler::startElement(const QName& qname, const AttributeMap& attributes) {
    _result.reset(new Tied());
    
    auto number = attributes.find(kNumberAttribute);
    if (number != attributes.end())
        _result->setNumber(lxml::IntegerHandler::parseInteger(number->second));
    
    auto type = attributes.find(kTypeAttribute);
    if (type != attributes.end())
        _result->setType(typeFromString(type->second));
    
    auto placement = attributes.find(kPlacementAttribute);
    if (placement != attributes.end())
        _result->setPlacement(placementFromString(placement->second));
    
    auto orientation = attributes.find(kOrientationAttribute);
    if (orientation != attributes.end())
        _result->setOrientation(orientationFromString(orientation->second));
}

dom::StartStopContinue TiedHandler::typeFromString(const std::string& string) {
    if (string == "start")
        return dom::kStart;
    else if (string == "continue")
        return dom::kContinue;
    else if (string == "stop")
        return dom::kStop;
    throw dom::InvalidDataError("Invalid slur type " + string);
}

dom::Placement TiedHandler::placementFromString(const std::string& string) {
    if (string == "above")
        return dom::Placement::Above;
    else if (string == "below")
        return dom::Placement::Below;
    throw dom::InvalidDataError("Invalid slur placement " + string);
}

dom::Orientation TiedHandler::orientationFromString(const std::string& string) {
    if (string == "over")
        return dom::Orientation::Over;
    else if (string == "under")
        return dom::Orientation::Under;
    throw dom::InvalidDataError("Invalid slur orientation " + string);
}

} // namespace mxml
