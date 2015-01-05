//  Created by Alejandro Isaza on 2014-06-23.
//  Copyright (c) 2014 Venture Media Labs. All rights reserved.

#include "EmptyPlacementHandler.h"
#include "TurnHandler.h"

namespace mxml {

using dom::Turn;

static const char* kPlacementAttribute = "placement";
static const char* kSlashAttribute = "slash";
    
void TurnHandler::startElement(const lxml::QName& qname, const AttributeMap& attributes) {
    _result.reset(new Turn());

    auto placement = attributes.find(kPlacementAttribute);
    if (placement != attributes.end())
        _result->setPlacement(presentOptional(EmptyPlacementHandler::placementFromString(placement->second)));

    auto slash = attributes.find(kSlashAttribute);
    if (slash != attributes.end())
        _result->setSlash(slash->second == "yes");
}

} // namespace
