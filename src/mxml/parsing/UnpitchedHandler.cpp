//  Created by Alejandro Isaza on 2/20/2014.
//  Copyright (c) 2014 Venture Media Labs Inc. All rights reserved.

#include "UnpitchedHandler.h"
#include "PitchHandler.h"
#include <cstring>

namespace mxml {

using lxml::QName;

static const char* kDisplayStepTag = "display-step";
static const char* kDisplayOctaveTag = "display-octave";

lxml::RecursiveHandler* UnpitchedHandler::startSubElement(const QName& qname) {
    if (strcmp(qname.localName(), kDisplayStepTag) == 0)
        return &_stringHandler;
    else if (strcmp(qname.localName(), kDisplayOctaveTag) == 0)
        return &_integerHandler;
    return 0;
}

void UnpitchedHandler::endSubElement(const QName& qname, RecursiveHandler* parser) {
    if (strcmp(qname.localName(), kDisplayStepTag) == 0)
        _result.setDisplayStep(PitchHandler::stepFromString(_stringHandler.result()));
    else if (strcmp(qname.localName(), kDisplayOctaveTag) == 0)
        _result.setDisplayOctave(_integerHandler.result());
}

} // namespace mxml
