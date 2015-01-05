//  Created by Alejandro Isaza on 2014-04-23.
//  Copyright (c) 2014 Venture Media Labs. All rights reserved.

#pragma once
#include <lxml/BaseRecursiveHandler.h>
#include "ArticulationHandler.h"
#include <lxml/ListHandler.h>
#include "FermataHandler.h"
#include "OrnamentsHandler.h"
#include "SlurHandler.h"
#include "TiedHandler.h"

#include <mxml/dom/Notations.h>

namespace mxml {

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
};

} // namespace mxml
