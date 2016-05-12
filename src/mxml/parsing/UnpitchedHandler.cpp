// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "UnpitchedHandler.h"
#include "PitchHandler.h"
#include <cstring>

namespace mxml {

using dom::Unpitched;
using lxml::QName;

static const char* kDisplayStepTag = "display-step";
static const char* kDisplayOctaveTag = "display-octave";

void UnpitchedHandler::startElement(const lxml::QName& qname, const AttributeMap& attributes) {
    _result.reset(new Unpitched());
}

lxml::RecursiveHandler* UnpitchedHandler::startSubElement(const QName& qname) {
    if (strcmp(qname.localName(), kDisplayStepTag) == 0)
        return &_stringHandler;
    else if (strcmp(qname.localName(), kDisplayOctaveTag) == 0)
        return &_integerHandler;
    return 0;
}

void UnpitchedHandler::endSubElement(const QName& qname, RecursiveHandler* parser) {
    if (strcmp(qname.localName(), kDisplayStepTag) == 0)
        _result->setDisplayStep(PitchHandler::stepFromString(_stringHandler.result()));
    else if (strcmp(qname.localName(), kDisplayOctaveTag) == 0)
        _result->setDisplayOctave(_integerHandler.result());
}

} // namespace mxml
