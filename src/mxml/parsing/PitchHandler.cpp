// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "PitchHandler.h"
#include <mxml/dom/InvalidDataError.h>
#include <cstring>

namespace mxml {

using dom::Pitch;
using lxml::QName;

static const char* kStepTag = "step";
static const char* kAlterTag = "alter";
static const char* kOctaveTag = "octave";

void PitchHandler::startElement(const lxml::QName& qname, const AttributeMap& attributes) {
    _result.reset(new Pitch());
}

lxml::RecursiveHandler* PitchHandler::startSubElement(const QName& qname) {
    if (strcmp(qname.localName(), kStepTag) == 0)
        return &_stringHandler;
    else if (strcmp(qname.localName(), kAlterTag) == 0)
        return &_integerHandler;
    else if (strcmp(qname.localName(), kOctaveTag) == 0)
        return &_integerHandler;
    return 0;
}

void PitchHandler::endSubElement(const QName& qname, RecursiveHandler* parser) {
    if (strcmp(qname.localName(), kStepTag) == 0)
        _result->setStep(stepFromString(_stringHandler.result()));
    else if (strcmp(qname.localName(), kAlterTag) == 0)
        _result->setAlter(_integerHandler.result());
    else if (strcmp(qname.localName(), kOctaveTag) == 0)
        _result->setOctave(_integerHandler.result());
}

Pitch::Step PitchHandler::stepFromString(const std::string& string) {
    char c = string[0];
    switch (c) {
        case 'C':
        case 'c':
            return Pitch::Step::C;
        case 'D':
        case 'd':
            return Pitch::Step::D;
        case 'E':
        case 'e':
            return Pitch::Step::E;
        case 'F':
        case 'f':
            return Pitch::Step::F;
        case 'G':
        case 'g':
            return Pitch::Step::G;
        case 'A':
        case 'a':
            return Pitch::Step::A;
        case 'B':
        case 'b':
            return Pitch::Step::B;
            
        default:
            throw dom::InvalidDataError("Invalid step " + string);
    }
}

} // namespace mxml
