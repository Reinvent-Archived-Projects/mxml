// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "SoundHandler.h"
#include <lxml/DoubleHandler.h>
#include <cstring>

namespace mxml {

using lxml::QName;

using namespace dom;
using namespace lxml;

static const char* kTempoAttribute = "tempo";
static const char* kDynamicsAttribute = "dynamics";
static const char* kDaCapoAttribute = "dacapo";
static const char* kSegnoAttribute = "segno";
static const char* kDalSegnoAttribute = "dalsegno";
static const char* kCodaAttribute = "coda";
static const char* kToCodaAttribute = "tocoda";
static const char* kDivisionsAttribute = "divisions";
static const char* kForwardRepeatAttribute = "forward-repeat";
static const char* kFineAttribute = "fine";
static const char* kTimeOnlyAttribute = "time-only";
static const char* kPizzicatoAttribute = "pizzicato";
//static const char* kPanAttribute = "pan";
//static const char* kElevationAttribute = "elevation";
//static const char* kDamperPedalAttribute = "damper-pedal";
//static const char* kSoftPedalAttribute = "soft-pedal";
//static const char* kSostenutoPedalAttribute = "sostenuto-pedal";

void SoundHandler::startElement(const QName& qname, const AttributeMap& attributes) {
    _result.reset(new Sound());
    
    auto tempo = attributes.find(kTempoAttribute);
    if (tempo != attributes.end())
        _result->tempo = presentOptional(static_cast<float>(DoubleHandler::parseDouble(tempo->second)));
    
    auto dynamics = attributes.find(kDynamicsAttribute);
    if (dynamics != attributes.end())
        _result->dynamics = presentOptional(static_cast<float>(DoubleHandler::parseDouble(dynamics->second)));

    auto dacapo = attributes.find(kDaCapoAttribute);
    if (dacapo != attributes.end() && dacapo->second == "yes")
        _result->dacapo = true;

    auto segno = attributes.find(kSegnoAttribute);
    if (segno != attributes.end())
        _result->segno = presentOptional(segno->second);

    auto dalsegno = attributes.find(kDalSegnoAttribute);
    if (dalsegno != attributes.end())
        _result->dalsegno = presentOptional(dalsegno->second);

    auto coda = attributes.find(kCodaAttribute);
    if (coda != attributes.end())
        _result->coda = presentOptional(coda->second);

    auto tocoda = attributes.find(kToCodaAttribute);
    if (tocoda != attributes.end())
        _result->tocoda = presentOptional(tocoda->second);

    auto divisions = attributes.find(kDivisionsAttribute);
    if (divisions != attributes.end())
        _result->divisions = presentOptional(static_cast<float>(DoubleHandler::parseDouble(divisions->second)));

    auto forwardRepeat = attributes.find(kForwardRepeatAttribute);
    if (forwardRepeat != attributes.end() && forwardRepeat->second == "yes")
        _result->forwardRepeat = true;

    auto fine = attributes.find(kFineAttribute);
    if (fine != attributes.end())
        _result->fine = presentOptional(fine->second);

    auto timeOnly = attributes.find(kTimeOnlyAttribute);
    if (timeOnly != attributes.end())
        _result->timeOnly = presentOptional(timeOnly->second);

    auto pizzicato = attributes.find(kPizzicatoAttribute);
    if (pizzicato != attributes.end() && pizzicato->second == "yes")
        _result->pizzicato = true;

    // TODO:
    //    <xs:attribute name="pan" type="rotation-degrees"/>
    //    <xs:attribute name="elevation" type="rotation-degrees"/>
    //    <xs:attribute name="damper-pedal" type="yes-no-number"/>
    //    <xs:attribute name="soft-pedal" type="yes-no-number"/>
    //    <xs:attribute name="sostenuto-pedal" type="yes-no-number"/>
}

} // namespace mxml
