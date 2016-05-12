// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "TupletHandler.h"

#include "EmptyPlacementHandler.h"
#include "TypeFactories.h"
#include "PositionFactory.h"
#include "NoteHandler.h"


namespace mxml {
namespace parsing {

static const char* kBracketAttribute = "bracket";
static const char* kNumberAttribute = "number";
static const char* kPlacementAttribute = "placement";
static const char* kShowNumberAttribute = "show-number";
static const char* kShowTypeAttribute = "show-type";
static const char* kTypeAttribute = "type";

static const char* kActualTag = "tuplet-actual";
static const char* kNormalTag = "tuplet-normal";


void TupletHandler::startElement(const lxml::QName& qname, const AttributeMap& attributes) {
    _result.reset(new dom::Tuplet{});

    _result->position = PositionFactory::buildFromAttributes(attributes);

    auto bracket = attributes.find(kBracketAttribute);
    if (bracket != attributes.end())
        _result->bracket = {Factory::yesNoValue(bracket->second), true};

    auto number = attributes.find(kNumberAttribute);
    if (number != attributes.end())
        _result->number = lxml::IntegerHandler::parseInteger(number->second);

    auto placement = attributes.find(kPlacementAttribute);
    if (placement != attributes.end())
        _result->placement = presentOptional(EmptyPlacementHandler::placementFromString(placement->second));

    auto showNumber = attributes.find(kShowNumberAttribute);
    if (showNumber != attributes.end())
        _result->showNumber = showFromString(showNumber->second);

    auto showType = attributes.find(kShowTypeAttribute);
    if (showType != attributes.end())
        _result->showType = showFromString(showType->second);

    auto type = attributes.find(kTypeAttribute);
    if (type != attributes.end())
        _result->type = typeFromString(type->second);
}

lxml::RecursiveHandler* TupletHandler::startSubElement(const lxml::QName& qname) {
    if (strcmp(qname.localName(), kActualTag) == 0)
        return &_genericNodeHandler;
    else if (strcmp(qname.localName(), kNormalTag) == 0)
        return &_genericNodeHandler;
    return 0;
}

void TupletHandler::endSubElement(const lxml::QName& qname, RecursiveHandler* parser) {
    if (strcmp(qname.localName(), kActualTag) == 0) {
        auto node = _genericNodeHandler.result();
        auto number = node->child("tuplet-number");
        if (number)
            _result->actual.number = dom::presentOptional(lxml::IntegerHandler::parseInteger(number->text()));
        auto type = node->child("tuplet-type");
        if (type)
            _result->actual.type = dom::presentOptional(NoteHandler::typeFromString(type->text()));
    } else if (strcmp(qname.localName(), kNormalTag) == 0) {
        auto node = _genericNodeHandler.result();
        auto number = node->child("tuplet-number");
        if (number)
            _result->normal.number = dom::presentOptional(lxml::IntegerHandler::parseInteger(number->text()));
        auto type = node->child("tuplet-type");
        if (type)
            _result->normal.type = dom::presentOptional(NoteHandler::typeFromString(type->text()));
    }
}

dom::Tuplet::Type TupletHandler::typeFromString(const std::string& string) {
    if (string == "start")
        return dom::Tuplet::Type::Start;
    if (string == "stop")
        return dom::Tuplet::Type::Stop;
    return dom::Tuplet::Type::Start;
}

dom::Tuplet::Show TupletHandler::showFromString(const std::string& string) {
    if (string == "actual")
        return dom::Tuplet::Show::Actual;
    if (string == "both")
        return dom::Tuplet::Show::Both;
    if (string == "none")
        return dom::Tuplet::Show::None;
    return dom::Tuplet::Show::Actual;
}

} // namespace parsing
} // namespace mxml
