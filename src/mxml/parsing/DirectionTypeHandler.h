// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once
#include <mxml/dom/DirectionType.h>
#include <mxml/dom/Types.h>
#include <mxml/dom/Pedal.h>
#include <mxml/dom/Wedge.h>

#include <lxml/BaseRecursiveHandler.h>
#include <lxml/DoubleHandler.h>
#include <lxml/IntegerHandler.h>
#include <lxml/StringHandler.h>

#include <memory>

#include "OctaveShiftHandler.h"


namespace mxml {

class DynamicsHandler : public lxml::BaseRecursiveHandler<std::unique_ptr<dom::Dynamics>> {
public:
    void startElement(const lxml::QName& qname, const AttributeMap& attributes);
    RecursiveHandler* startSubElement(const lxml::QName& qname);
};

class WedgeHandler : public lxml::BaseRecursiveHandler<std::unique_ptr<dom::Wedge>> {
public:
    void startElement(const lxml::QName& qname, const AttributeMap& attributes);
    
    static dom::Wedge::Type typeFromString(const std::string& string);
    
private:
    lxml::IntegerHandler _integerHandler;
};

class PedalHandler : public lxml::BaseRecursiveHandler<std::unique_ptr<dom::Pedal>> {
public:
    void startElement(const lxml::QName& qname, const AttributeMap& attributes);

    static dom::StartStopContinue typeFromString(const std::string& string);
};

class WordsHandler : public lxml::BaseRecursiveHandler<std::unique_ptr<dom::Words>> {
public:
    void startElement(const lxml::QName& qname, const AttributeMap& attributes);
    void endElement(const lxml::QName& qname, const std::string& contents);
};

class SegnoHandler : public lxml::BaseRecursiveHandler<std::unique_ptr<dom::Segno>> {
public:
    void startElement(const lxml::QName& qname, const AttributeMap& attributes) {
        _result.reset(new dom::Segno());
    }
};

class CodaHandler : public lxml::BaseRecursiveHandler<std::unique_ptr<dom::Coda>> {
public:
    void startElement(const lxml::QName& qname, const AttributeMap& attributes) {
        _result.reset(new dom::Coda());
    }
};

class DirectionTypeHandler : public lxml::BaseRecursiveHandler<std::unique_ptr<dom::DirectionType>> {
public:
    void startElement(const lxml::QName& qname, const AttributeMap& attributes);
    
    RecursiveHandler* startSubElement(const lxml::QName& qname);
    void endSubElement(const lxml::QName& qname, RecursiveHandler* parser);
    
private:
    DynamicsHandler _dynamicsHandler;
    PedalHandler _pedalHandler;
    WedgeHandler _wedgeHandler;
    WordsHandler _wordsHandler;
    SegnoHandler _segnoHandler;
    CodaHandler _codaHandler;
    OctaveShiftHandler _octaveShiftHandler;
};

} // namespace mxml
