// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "MordentHandler.h"
#include "EmptyPlacementHandler.h"

namespace mxml {

using dom::Mordent;

static const char* kPlacementAttribute = "placement";
static const char* kLongAttribute = "long";

void MordentHandler::startElement(const lxml::QName& qname, const AttributeMap& attributes) {
    _result.reset(new Mordent());

    auto placement = attributes.find(kPlacementAttribute);
    if (placement != attributes.end())
        _result->setPlacement(presentOptional(EmptyPlacementHandler::placementFromString(placement->second)));

    auto longv = attributes.find(kLongAttribute);
    if (longv != attributes.end())
        _result->setLong(longv->second == "yes");
}


} // namespace
