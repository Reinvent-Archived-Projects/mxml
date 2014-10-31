//  Created by Alejandro Isaza on 2014-03-20.
//  Copyright (c) 2014 Venture Media Labs. All rights reserved.

#pragma once
#include "dom/DirectionType.h"
#include "dom/Types.h"
#include "dom/Pedal.h"
#include "dom/Wedge.h"

#include "BaseRecursiveHandler.h"
#include "DoubleHandler.h"
#include "IntegerHandler.h"
#include "StringHandler.h"

#include <memory>

namespace mxml {

using std::unique_ptr;

class DynamicsHandler : public lxml::BaseRecursiveHandler<unique_ptr<dom::Dynamics>> {
public:
    void startElement(const lxml::QName& qname, const AttributeMap& attributes);
    RecursiveHandler* startSubElement(const lxml::QName& qname);
};

class WedgeHandler : public lxml::BaseRecursiveHandler<unique_ptr<dom::Wedge>> {
public:
    void startElement(const lxml::QName& qname, const AttributeMap& attributes);
    
    static dom::Wedge::Type typeFromString(const std::string& string);
    
private:
    lxml::IntegerHandler _integerHandler;
};

class PedalHandler : public lxml::BaseRecursiveHandler<unique_ptr<dom::Pedal>> {
public:
    void startElement(const lxml::QName& qname, const AttributeMap& attributes);

    static dom::StartStopContinue typeFromString(const std::string& string);
};

class WordsHandler : public lxml::BaseRecursiveHandler<unique_ptr<dom::Words>> {
public:
    void startElement(const lxml::QName& qname, const AttributeMap& attributes);
    void endElement(const lxml::QName& qname, const std::string& contents);
};

class DirectionTypeHandler : public lxml::BaseRecursiveHandler<unique_ptr<dom::DirectionType>> {
public:
    void startElement(const lxml::QName& qname, const AttributeMap& attributes);
    
    RecursiveHandler* startSubElement(const lxml::QName& qname);
    void endSubElement(const lxml::QName& qname, RecursiveHandler* parser);
    
private:
    DynamicsHandler _dynamicsHandler;
    PedalHandler _pedalHandler;
    WedgeHandler _wedgeHandler;
    WordsHandler _wordsHandler;
};

} // namespace mxml
