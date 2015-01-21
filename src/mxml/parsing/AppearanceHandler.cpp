//  Copyright (c) 2015 Venture Media Labs. All rights reserved.

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
    if (string == "beam")           return dom::kLineTypeBeam;
    if (string == "bracket")        return dom::kLineTypeBracket;
    if (string == "dashes")         return dom::kLineTypeDashes;
    if (string == "enclosure")      return dom::kLineTypeEnclosure;
    if (string == "ending")         return dom::kLineTypeEnding;
    if (string == "extend")         return dom::kLineTypeExtend;
    if (string == "heavy barline")  return dom::kLineTypeHeavyBarline;
    if (string == "leger")          return dom::kLineTypeLeger;
    if (string == "light barline")  return dom::kLineTypeLightBarline;
    if (string == "octave shift")   return dom::kLineTypeOctaveShift;
    if (string == "pedal")          return dom::kLineTypePedal;
    if (string == "slur middle")    return dom::kLineTypeSlurMiddle;
    if (string == "slur tip")       return dom::kLineTypeSlurTip;
    if (string == "staff")          return dom::kLineTypeStaff;
    if (string == "stem")           return dom::kLineTypeStem;
    if (string == "tie middle")     return dom::kLineTypeTieMiddle;
    if (string == "tie tip")        return dom::kLineTypeTieTip;
    if (string == "tuplet bracket") return dom::kLineTypeTupletBracket;
    if (string == "wedge")          return dom::kLineTypeWedge;

    throw dom::InvalidDataError("Invalid halign type " + string);
}

dom::NoteType AppearanceHandler::noteTypeFromString(const std::string& string) {
    if (string == "cue")   return dom::kNoteTypeCue;
    if (string == "grace") return dom::kNoteTypeGrace;
    if (string == "large") return dom::kNoteTypeLarge;

    throw dom::InvalidDataError("Invalid halign type " + string);
}

dom::DistanceType AppearanceHandler::distanceTypeFromString(const std::string& string) {
    if (string == "beam")   return dom::kDistanceTypeBeam;
    if (string == "hyphen") return dom::kDistanceTypeHyphen;
    
    throw dom::InvalidDataError("Invalid halign type " + string);
}

} // namespace mxml
