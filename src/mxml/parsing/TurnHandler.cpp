// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

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
