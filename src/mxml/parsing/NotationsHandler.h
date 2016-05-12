// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once
#include <lxml/BaseRecursiveHandler.h>
#include "ArticulationHandler.h"
#include <lxml/ListHandler.h>
#include "FermataHandler.h"
#include "OrnamentsHandler.h"
#include "SlurHandler.h"
#include "TiedHandler.h"
#include "TupletHandler.h"

#include <mxml/dom/Notations.h>

namespace mxml {
namespace parsing {

class NotationsHandler : public lxml::BaseRecursiveHandler<std::unique_ptr<dom::Notations>> {
public:
    NotationsHandler();
    
    void startElement(const lxml::QName& qname, const AttributeMap& attributes);
    RecursiveHandler* startSubElement(const lxml::QName& qname);
    void endSubElement(const lxml::QName& qname, RecursiveHandler* parser);
    
private:
    ArticulationHandler _articulationHandler;
    lxml::ListHandler<std::unique_ptr<dom::Articulation>> _articulationsHandler;
    FermataHandler _fermataHandler;
    OrnamentsHandler _ornamentsHandler;
    SlurHandler _slurHandler;
    TiedHandler _tiedHandler;
    TupletHandler _tupletHandler;
};

} // namespace parsing
} // namespace mxml
