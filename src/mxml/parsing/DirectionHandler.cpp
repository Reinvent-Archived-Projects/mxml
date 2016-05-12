// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "DirectionHandler.h"
#include <cstring>

namespace mxml {

using dom::Direction;
using dom::Placement;
using lxml::QName;

static const char* kPlacementAttribute = "placement";
static const char* kStaffTag = "staff";
static const char* kOffsetTag = "offset";
static const char* kDirectionTypeTag = "direction-type";
static const char* kSoundTag = "sound";

void DirectionHandler::startElement(const QName& qname, const AttributeMap& attributes) {
    _result.reset(new Direction());
    
    auto placement = attributes.find(kPlacementAttribute);
    if (placement != attributes.end())
        _result->setPlacement(presentOptional(placementFromString(placement->second)));
}

lxml::RecursiveHandler* DirectionHandler::startSubElement(const QName& qname) {
    if (strcmp(qname.localName(), kStaffTag) == 0)
        return &_integerHandler;
    else if (strcmp(qname.localName(), kOffsetTag) == 0)
        return &_doubleHandler;
    else if (strcmp(qname.localName(), kDirectionTypeTag) == 0)
        return &_directionTypeHandler;
    else if (strcmp(qname.localName(), kSoundTag) == 0)
        return &_soundHandler;
    return 0;
}

void DirectionHandler::endSubElement(const QName& qname, RecursiveHandler* parser) {
    using dom::presentOptional;

    if (strcmp(qname.localName(), kStaffTag) == 0)
        _result->setStaff(dom::presentOptional(_integerHandler.result()));
    else if (strcmp(qname.localName(), kOffsetTag) == 0)
        _result->setOffset(presentOptional((float)_doubleHandler.result()));
    else if (strcmp(qname.localName(), kDirectionTypeTag) == 0)
        _result->setType(_directionTypeHandler.result());
    else if (strcmp(qname.localName(), kSoundTag) == 0)
        _result->setSound(_soundHandler.result());
}

Placement DirectionHandler::placementFromString(const std::string& string) {
    if (string == "below")
        return dom::Placement::Below;
    return dom::Placement::Above;
}

} // namespace mxml
