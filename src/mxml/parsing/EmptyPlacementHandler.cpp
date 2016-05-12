// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "EmptyPlacementHandler.h"
#include <mxml/dom/InvalidDataError.h>

namespace mxml {

using dom::EmptyPlacement;
using dom::Placement;
using lxml::QName;

static const char* kPlacementAttribute = "placement";

void EmptyPlacementHandler::startElement(const QName& qname, const AttributeMap& attributes) {
    _result.reset(new EmptyPlacement());
    
    auto placement = attributes.find(kPlacementAttribute);
    if (placement != attributes.end())
        _result->setPlacement(presentOptional(placementFromString(placement->second)));
}

Placement EmptyPlacementHandler::placementFromString(const std::string& string) {
    if (string == "above")
        return dom::Placement::Above;
    else if (string == "below")
        return dom::Placement::Below;
    throw dom::InvalidDataError("Invalid placement " + string);
}

} // namespace mxml
