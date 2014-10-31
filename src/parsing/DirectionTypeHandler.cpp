//  Created by Alejandro Isaza on 2014-03-20.
//  Copyright (c) 2014 Venture Media Labs. All rights reserved.

#include "DirectionTypeHandler.h"
#include "DoubleHandler.h"
#include "dom/InvalidDataError.h"
#include <cstring>

namespace mxml {

using dom::Dynamics;
using dom::Pedal;
using dom::Wedge;
using dom::Words;
using lxml::QName;

static const char* kDefaultXAttribute = "default-x";
static const char* kDefaultYAttribute = "default-y";
static const char* kTypeAttribute = "type";
static const char* kNumberAttribute = "number";
static const char* kSpreadAttribute = "spread";
static const char* kLineAttribute = "line";
static const char* kSignAttribute = "sign";

static const char* kDynamicsTag = "dynamics";
static const char* kPedalTag = "pedal";
static const char* kWedgeTag = "wedge";
static const char* kWordsTag = "words";

void DynamicsHandler::startElement(const QName& qname, const AttributeMap& attributes) {
    using lxml::DoubleHandler;
    using dom::presentOptional;

    _result.reset(new Dynamics());
    
    auto defaultX = attributes.find(QName(kDefaultXAttribute, 0, 0));
    if (defaultX != attributes.end())
        _result->setDefaultX(presentOptional((float)DoubleHandler::parseDouble(defaultX->second)));
    
    auto defaultY = attributes.find(QName(kDefaultYAttribute, 0, 0));
    if (defaultY != attributes.end())
        _result->setDefaultY(presentOptional((float)DoubleHandler::parseDouble(defaultY->second)));
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

    _result.reset(new Wedge());
    
    auto defaultX = attributes.find(kDefaultXAttribute);
    if (defaultX != attributes.end())
        _result->setDefaultX(presentOptional((float)DoubleHandler::parseDouble(defaultX->second)));
    
    auto defaultY = attributes.find(kDefaultYAttribute);
    if (defaultY != attributes.end())
        _result->setDefaultY(presentOptional((float)DoubleHandler::parseDouble(defaultY->second)));
    
    auto type = attributes.find(kTypeAttribute);
    if (type != attributes.end())
        _result->setType(typeFromString(type->second));
    
    auto number = attributes.find(kNumberAttribute);
    if (number != attributes.end())
        _result->setNumber(lxml::IntegerHandler::parseInteger(number->second));
    
    auto spread = attributes.find(kSpreadAttribute);
    if (spread != attributes.end())
        _result->setSpread(DoubleHandler::parseDouble(spread->second));
}

Wedge::Type WedgeHandler::typeFromString(const std::string& string) {
    if (string == "crescendo")
        return Wedge::TYPE_CRESCENDO;
    if (string == "diminuendo")
        return Wedge::TYPE_DIMINUENDO;
    if (string == "stop")
        return Wedge::TYPE_STOP;
    if (string == "continue")
        return Wedge::TYPE_CONTINUE;
    return Wedge::TYPE_CRESCENDO;
}


void PedalHandler::startElement(const lxml::QName& qname, const AttributeMap& attributes) {
    using dom::presentOptional;
    using lxml::DoubleHandler;
    using lxml::StringHandler;

    _result.reset(new Pedal());

    auto defaultX = attributes.find(kDefaultXAttribute);
    if (defaultX != attributes.end())
        _result->setDefaultX(presentOptional((float)DoubleHandler::parseDouble(defaultX->second)));

    auto defaultY = attributes.find(kDefaultYAttribute);
    if (defaultY != attributes.end())
        _result->setDefaultY(presentOptional((float)DoubleHandler::parseDouble(defaultY->second)));

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
        return dom::TYPE_START;
    if (string == "stop")
        return dom::TYPE_STOP;
    if (string == "continue")
        return dom::TYPE_CONTINUE;
    throw dom::InvalidDataError("Invalid type " + string);
}

void WordsHandler::startElement(const QName& qname, const AttributeMap& attributes) {
    using dom::presentOptional;
    using lxml::DoubleHandler;

    _result.reset(new Words());
    
    auto defaultX = attributes.find(QName(kDefaultXAttribute, 0, 0));
    if (defaultX != attributes.end())
        _result->setDefaultX(presentOptional((float)DoubleHandler::parseDouble(defaultX->second)));
    
    auto defaultY = attributes.find(QName(kDefaultYAttribute, 0, 0));
    if (defaultY != attributes.end())
        _result->setDefaultY(presentOptional((float)DoubleHandler::parseDouble(defaultY->second)));
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
}

} // namespace mxml
