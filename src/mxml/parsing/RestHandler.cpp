//  Created by Alejandro Isaza on 2/20/2014.
//  Copyright (c) 2014 Venture Media Labs Inc. All rights reserved.

#include "RestHandler.h"
#include "PitchHandler.h"
#include <cstring>

namespace mxml {

using dom::Rest;
using lxml::QName;

static const char* kDisplayStepTag = "display-step";
static const char* kDisplayOctaveTag = "display-octave";

void RestHandler::startElement(const QName& qname, const AttributeMap& attributes) {
    _result.reset(new Rest());
}

lxml::RecursiveHandler* RestHandler::startSubElement(const QName& qname) {
    if (strcmp(qname.localName(), kDisplayStepTag) == 0)
        return &_stringHandler;
    else if (strcmp(qname.localName(), kDisplayOctaveTag) == 0)
        return &_integerHandler;
    return 0;
}

void RestHandler::endSubElement(const QName& qname, RecursiveHandler* parser) {
    using dom::presentOptional;

    if (strcmp(qname.localName(), kDisplayStepTag) == 0)
        _result->setDisplayStep(presentOptional(PitchHandler::stepFromString(_stringHandler.result())));
    else if (strcmp(qname.localName(), kDisplayOctaveTag) == 0)
        _result->setDisplayOctave(presentOptional(_integerHandler.result()));
}

} // namespace mxml
