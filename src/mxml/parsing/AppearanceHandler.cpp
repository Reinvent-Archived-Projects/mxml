// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "AppearanceHandler.h"

#include <lxml/DoubleHandler.h>
#include <mxml/dom/InvalidDataError.h>
#include <cstring>


namespace mxml {

static const char* kLineWidthTag = "line-width";
static const char* kNoteSizeTag = "note-size";
static const char* kDistanceTag = "distance";

void AppearanceHandler::startElement(const lxml::QName& qname, const AttributeMap& attributes) {
    _result = dom::Appearance{};
}

lxml::RecursiveHandler* AppearanceHandler::startSubElement(const lxml::QName& qname) {
    if (strcmp(qname.localName(), kLineWidthTag) == 0)
        return &_handler;
    else if (strcmp(qname.localName(), kNoteSizeTag) == 0)
        return &_handler;
    else if (strcmp(qname.localName(), kDistanceTag) == 0)
        return &_handler;
    return 0;
}

void AppearanceHandler::endSubElement(const lxml::QName& qname, RecursiveHandler* parser) {
    if (strcmp(qname.localName(), kLineWidthTag) == 0) {
        auto lineWidthNode = _handler.result();
        auto type = lineTypeFromString(lineWidthNode->attribute("type"));
        auto value = lxml::DoubleHandler::parseDouble(lineWidthNode->text());
        _result.lineWidths[type] = static_cast<dom::tenths_t>(value);
    } else if (strcmp(qname.localName(), kNoteSizeTag) == 0) {
        auto sizeNode = _handler.result();
        auto type = noteTypeFromString(sizeNode->attribute("type"));
        auto value = lxml::DoubleHandler::parseDouble(sizeNode->text());
        _result.noteSizes[type] = static_cast<dom::tenths_t>(value);
    } else if (strcmp(qname.localName(), kDistanceTag) == 0) {
        auto distanceNode = _handler.result();
        auto type = distanceTypeFromString(distanceNode->attribute("type"));
        auto value = lxml::DoubleHandler::parseDouble(distanceNode->text());
        _result.distances[type] = static_cast<dom::tenths_t>(value);
    }
}

dom::LineType AppearanceHandler::lineTypeFromString(const std::string& string) {
    if (string == "beam")           return dom::LineType::Beam;
    if (string == "bracket")        return dom::LineType::Bracket;
    if (string == "dashes")         return dom::LineType::Dashes;
    if (string == "enclosure")      return dom::LineType::Enclosure;
    if (string == "ending")         return dom::LineType::Ending;
    if (string == "extend")         return dom::LineType::Extend;
    if (string == "heavy barline")  return dom::LineType::HeavyBarline;
    if (string == "leger")          return dom::LineType::Leger;
    if (string == "light barline")  return dom::LineType::LightBarline;
    if (string == "octave shift")   return dom::LineType::OctaveShift;
    if (string == "pedal")          return dom::LineType::Pedal;
    if (string == "slur middle")    return dom::LineType::SlurMiddle;
    if (string == "slur tip")       return dom::LineType::SlurTip;
    if (string == "staff")          return dom::LineType::Staff;
    if (string == "stem")           return dom::LineType::Stem;
    if (string == "tie middle")     return dom::LineType::TieMiddle;
    if (string == "tie tip")        return dom::LineType::TieTip;
    if (string == "tuplet bracket") return dom::LineType::TupletBracket;
    if (string == "wedge")          return dom::LineType::Wedge;

    throw dom::InvalidDataError("Invalid halign type " + string);
}

dom::NoteType AppearanceHandler::noteTypeFromString(const std::string& string) {
    if (string == "cue")   return dom::NoteType::Cue;
    if (string == "grace") return dom::NoteType::Grace;
    if (string == "large") return dom::NoteType::Large;

    throw dom::InvalidDataError("Invalid halign type " + string);
}

dom::DistanceType AppearanceHandler::distanceTypeFromString(const std::string& string) {
    if (string == "beam")   return dom::DistanceType::Beam;
    if (string == "hyphen") return dom::DistanceType::Hyphen;
    
    throw dom::InvalidDataError("Invalid halign type " + string);
}

} // namespace mxml
