//  Created by Alejandro Isaza on 2014-04-23.
//  Copyright (c) 2014 Venture Media Labs. All rights reserved.

#include "ArticulationHandler.h"
#include "EmptyPlacementHandler.h"
#include <mxml/dom/InvalidDataError.h>

namespace mxml {

using dom::Articulation;
using lxml::QName;

static const char* kPlacementAttribute = "placement";
const std::string ArticulationHandler::kTagNames[] = {
    "accent",
    "breath-mark",
    "caesura",
    "detached-legato",
    "doit",
    "falloff",
    "plop",
    "scoop",
    "spiccato",
    "staccatissimo",
    "staccato",
    "stress",
    "strong-accent",
    "tenuto",
    "unstress"
};

void ArticulationHandler::startElement(const QName& qname, const AttributeMap& attributes) {
    _result = Articulation();
    _result.setType(typeFromString(qname.localName()));
    
    auto placement = attributes.find(kPlacementAttribute);
    if (placement != attributes.end())
        _result.setPlacement(presentOptional(EmptyPlacementHandler::placementFromString(placement->second)));
}

Articulation::Type ArticulationHandler::typeFromString(const std::string& string) {
    if (string == "accent") return Articulation::ACCENT;
    if (string == "breath-mark") return Articulation::BREATH_MARK;
    if (string == "caesura") return Articulation::CAESURA;
    if (string == "detached-legato") return Articulation::DETACHED_LEGATO;
    if (string == "doit") return Articulation::DOIT;
    if (string == "falloff") return Articulation::FALLOFF;
    if (string == "plop") return Articulation::PLOP;
    if (string == "scoop") return Articulation::SCOOP;
    if (string == "spiccato") return Articulation::SPICCATO;
    if (string == "staccatissimo") return Articulation::STACCATISSIMO;
    if (string == "staccato") return Articulation::STACCATO;
    if (string == "stress") return Articulation::STRESS;
    if (string == "strong-accent") return Articulation::STRONG_ACCENT;
    if (string == "tenuto") return Articulation::TENUTO;
    if (string == "unstress") return Articulation::UNSTRESS;
    throw dom::InvalidDataError("Invalid articulation type " + string);
}

} // namespace mxml
