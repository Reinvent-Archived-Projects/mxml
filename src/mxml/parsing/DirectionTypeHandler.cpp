// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <lxml/DoubleHandler.h>
#include <mxml/dom/InvalidDataError.h>
#include <cstring>

#include "DirectionTypeHandler.h"
#include "PositionFactory.h"


namespace mxml {

using namespace parsing;

using dom::Dynamics;
using dom::Pedal;
using dom::Wedge;
using dom::Words;
using lxml::QName;

static const char* kTypeAttribute = "type";
static const char* kNumberAttribute = "number";
static const char* kSpreadAttribute = "spread";
static const char* kLineAttribute = "line";
static const char* kSignAttribute = "sign";

static const char* kDynamicsTag = "dynamics";
static const char* kPedalTag = "pedal";
static const char* kWedgeTag = "wedge";
static const char* kWordsTag = "words";
static const char* kSegnoTag = "segno";
static const char* kCodaTag = "coda";
static const char* kOctaveShiftTag = "octave-shift";

void DynamicsHandler::startElement(const QName& qname, const AttributeMap& attributes) {
    _result.reset(new Dynamics{});
    _result->position = PositionFactory::buildFromAttributes(attributes);
}

lxml::RecursiveHandler* DynamicsHandler::startSubElement(const QName& qname) {
    bool valid =
        strcmp(qname.localName(), "f") == 0 ||
        strcmp(qname.localName(), "ff") == 0 ||
        strcmp(qname.localName(), "fff") == 0 ||
        strcmp(qname.localName(), "ffff") == 0 ||
        strcmp(qname.localName(), "fffff") == 0 ||
        strcmp(qname.localName(), "ffffff") == 0 ||
        strcmp(qname.localName(), "fp") == 0 ||
        strcmp(qname.localName(), "fz") == 0 ||
        strcmp(qname.localName(), "mf") == 0 ||
        strcmp(qname.localName(), "mp") == 0 ||
        strcmp(qname.localName(), "p") == 0 ||
        strcmp(qname.localName(), "pp") == 0 ||
        strcmp(qname.localName(), "ppp") == 0 ||
        strcmp(qname.localName(), "pppp") == 0 ||
        strcmp(qname.localName(), "ppppp") == 0 ||
        strcmp(qname.localName(), "pppppp") == 0 ||
        strcmp(qname.localName(), "rf") == 0 ||
        strcmp(qname.localName(), "rfz") == 0 ||
        strcmp(qname.localName(), "sf") == 0 ||
        strcmp(qname.localName(), "sffz") == 0 ||
        strcmp(qname.localName(), "sfp") == 0 ||
        strcmp(qname.localName(), "sfpp") == 0 ||
        strcmp(qname.localName(), "sfz") == 0;
    if (valid)
        _result->setString(qname.localName());
    return 0;
}


void WedgeHandler::startElement(const QName& qname, const AttributeMap& attributes) {
    using dom::presentOptional;
    using lxml::DoubleHandler;

    _result.reset(new Wedge{});
    _result->position = PositionFactory::buildFromAttributes(attributes);

    auto type = attributes.find(kTypeAttribute);
    if (type != attributes.end())
        _result->setType(typeFromString(type->second));
    
    auto number = attributes.find(kNumberAttribute);
    if (number != attributes.end())
        _result->setNumber(lxml::IntegerHandler::parseInteger(number->second));
    
    auto spread = attributes.find(kSpreadAttribute);
    if (spread != attributes.end()) {
        auto spreadValue = DoubleHandler::parseDouble(spread->second);
        if (spreadValue > 0)
            _result->setSpread(spreadValue);
    }
}

Wedge::Type WedgeHandler::typeFromString(const std::string& string) {
    if (string == "crescendo")
        return Wedge::Type::Crescendo;
    if (string == "diminuendo")
        return Wedge::Type::Diminuendo;
    if (string == "stop")
        return Wedge::Type::Stop;
    if (string == "continue")
        return Wedge::Type::Continue;
    return Wedge::Type::Crescendo;
}


void PedalHandler::startElement(const lxml::QName& qname, const AttributeMap& attributes) {
    using dom::presentOptional;
    using lxml::DoubleHandler;
    using lxml::StringHandler;

    _result.reset(new Pedal{});
    _result->position = PositionFactory::buildFromAttributes(attributes);

    auto type = attributes.find(kTypeAttribute);
    if (type != attributes.end())
        _result->setType(typeFromString(type->second));

    auto line = attributes.find(kLineAttribute);
    if (line != attributes.end())
        _result->setLine(presentOptional(line->second == "yes"));

    auto sign = attributes.find(kSignAttribute);
    if (sign != attributes.end())
        _result->setSign(presentOptional(sign->second == "yes"));
}

dom::StartStopContinue PedalHandler::typeFromString(const std::string& string) {
    if (string == "start")
        return dom::kStart;
    if (string == "stop")
        return dom::kStop;
    if (string == "continue")
        return dom::kContinue;
    if (string == "change")
        return dom::kChange;
    throw dom::InvalidDataError("Invalid type " + string);
}

void WordsHandler::startElement(const QName& qname, const AttributeMap& attributes) {
    _result.reset(new Words{});
    _result->position = PositionFactory::buildFromAttributes(attributes);
}

void WordsHandler::endElement(const QName& qname, const std::string& contents) {
    _result->setContents(lxml::StringHandler::trim(contents));
}


void DirectionTypeHandler::startElement(const QName& qname, const AttributeMap& attributes) {
    
}

lxml::RecursiveHandler* DirectionTypeHandler::startSubElement(const QName& qname) {
    if (strcmp(qname.localName(), kDynamicsTag) == 0)
        return &_dynamicsHandler;
    else if (strcmp(qname.localName(), kWedgeTag) == 0)
        return &_wedgeHandler;
    else if (strcmp(qname.localName(), kPedalTag) == 0)
        return &_pedalHandler;
    else if (strcmp(qname.localName(), kWordsTag) == 0)
        return &_wordsHandler;
    else if (strcmp(qname.localName(), kSegnoTag) == 0)
        return &_segnoHandler;
    else if (strcmp(qname.localName(), kCodaTag) == 0)
        return &_codaHandler;
    else if (strcmp(qname.localName(), kOctaveShiftTag) == 0)
        return &_octaveShiftHandler;
    return 0;
}

void DirectionTypeHandler::endSubElement(const QName& qname, RecursiveHandler* parser) {
    if (strcmp(qname.localName(), kDynamicsTag) == 0)
        _result = _dynamicsHandler.result();
    else if (strcmp(qname.localName(), kWedgeTag) == 0)
        _result = _wedgeHandler.result();
    else if (strcmp(qname.localName(), kPedalTag) == 0)
        _result = _pedalHandler.result();
    else if (strcmp(qname.localName(), kWordsTag) == 0)
        _result = _wordsHandler.result();
    else if (strcmp(qname.localName(), kSegnoTag) == 0)
        _result = _segnoHandler.result();
    else if (strcmp(qname.localName(), kCodaTag) == 0)
        _result = _codaHandler.result();
    else if (strcmp(qname.localName(), kOctaveShiftTag) == 0)
        _result = _octaveShiftHandler.result();
}

} // namespace mxml
