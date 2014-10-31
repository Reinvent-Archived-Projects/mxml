//  Created by Alejandro Isaza on 2014-04-21.
//  Copyright (c) 2014 Venture Media Labs. All rights reserved.

#include "EmptyPlacementHandler.h"
#include "dom/InvalidDataError.h"

namespace mxml {

using dom::EmptyPlacement;
using dom::Placement;
using lxml::QName;

static const char* kPlacementAttribute = "placement";

void EmptyPlacementHandler::startElement(const QName& qname, const AttributeMap& attributes) {
    _result = EmptyPlacement();
    
    auto placement = attributes.find(kPlacementAttribute);
    if (placement != attributes.end())
        _result.setPlacement(presentOptional(placementFromString(placement->second)));
}

Placement EmptyPlacementHandler::placementFromString(const std::string& string) {
    if (string == "above")
        return dom::PLACEMENT_ABOVE;
    else if (string == "below")
        return dom::PLACEMENT_BELOW;
    throw dom::InvalidDataError("Invalid placement " + string);
}

} // namespace mxml
