//  Created by Alejandro Isaza on 2014-04-23.
//  Copyright (c) 2014 Venture Media Labs. All rights reserved.

#include "NotationsHandler.h"

namespace mxml {

using dom::Notations;
using lxml::QName;

static const char* kPrintObjectAttribute = "print-object";
static const char* kArticulationsTag = "articulations";
static const char* kFermataTag = "fermata";
static const char* kSlurTag = "slur";
static const char* kOrnamentsTag = "ornaments";
static const char* kTiedTag = "tied";

NotationsHandler::NotationsHandler() : _articulationHandler(), _articulationsHandler(_articulationHandler) {}

void NotationsHandler::startElement(const QName& qname, const AttributeMap& attributes) {
    _result.reset(new Notations());

    auto print = attributes.find(kPrintObjectAttribute);
    if (print != attributes.end())
        _result->setPrintObject(print->second == "no" ? false : true);
}

lxml::RecursiveHandler* NotationsHandler::startSubElement(const QName& qname) {
    if (strcmp(qname.localName(), kArticulationsTag) == 0)
        return &_articulationsHandler;
    else if (strcmp(qname.localName(), kFermataTag) == 0)
        return &_fermataHandler;
    else if (strcmp(qname.localName(), kOrnamentsTag) == 0)
        return &_ornamentsHandler;
    else if (strcmp(qname.localName(), kSlurTag) == 0)
        return &_slurHandler;
    else if (strcmp(qname.localName(), kTiedTag) == 0)
        return &_tiedHandler;
    return 0;
}

void NotationsHandler::endSubElement(const QName& qname, RecursiveHandler* parser) {
    if (strcmp(qname.localName(), kArticulationsTag) == 0)
        _result->setArticulations(_articulationsHandler.result());
    else if (strcmp(qname.localName(), kFermataTag) == 0)
        _result->setFermata(_fermataHandler.result());
    else if (strcmp(qname.localName(), kOrnamentsTag) == 0)
        _result->addOrnaments(_ornamentsHandler.result());
    else if (strcmp(qname.localName(), kSlurTag) == 0)
        _result->addSlur(_slurHandler.result());
    else if (strcmp(qname.localName(), kTiedTag) == 0)
        _result->addTied(_tiedHandler.result());
}

} // namespace mxml
