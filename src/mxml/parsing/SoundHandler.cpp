//  Created by Alejandro Isaza on 2014-05-14.
//  Copyright (c) 2014 Venture Media Labs. All rights reserved.

#include "SoundHandler.h"
#include <lxml/DoubleHandler.h>
#include <cstring>

namespace mxml {

using lxml::QName;

using namespace dom;
using namespace lxml;

static const char* kTempoAttribute = "tempo";
static const char* kDynamicsAttribute = "dynamics";

void SoundHandler::startElement(const QName& qname, const AttributeMap& attributes) {
    _result = Sound();
    
    auto tempo = attributes.find(kTempoAttribute);
    if (tempo != attributes.end())
        _result.setTempo(presentOptional(static_cast<float>(DoubleHandler::parseDouble(tempo->second))));
    
    auto dynamics = attributes.find(kDynamicsAttribute);
    if (dynamics != attributes.end())
        _result.setDynamics(presentOptional(static_cast<float>(DoubleHandler::parseDouble(dynamics->second))));
}

} // namespace mxml
