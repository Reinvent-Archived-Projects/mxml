// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "NotationsHandler.h"
#include "TypeFactories.h"


namespace mxml {
namespace parsing {

using dom::Notations;
using lxml::QName;

static const char* kPrintObjectAttribute = "print-object";
static const char* kArticulationsTag = "articulations";
static const char* kFermataTag = "fermata";
static const char* kSlurTag = "slur";
static const char* kOrnamentsTag = "ornaments";
static const char* kTiedTag = "tied";
static const char* kTupletTag = "tuplet";

NotationsHandler::NotationsHandler() : _articulationHandler(), _articulationsHandler(_articulationHandler) {}

void NotationsHandler::startElement(const QName& qname, const AttributeMap& attributes) {
    _result.reset(new Notations());

    auto print = attributes.find(kPrintObjectAttribute);
    if (print != attributes.end())
        _result->printObject = Factory::yesNoValue(print->second);
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
    else if (strcmp(qname.localName(), kTupletTag) == 0)
        return &_tupletHandler;
    return 0;
}

void NotationsHandler::endSubElement(const QName& qname, RecursiveHandler* parser) {
    if (strcmp(qname.localName(), kArticulationsTag) == 0) {
        _result->articulations = _articulationsHandler.result();
    } else if (strcmp(qname.localName(), kFermataTag) == 0) {
        _result->fermata = _fermataHandler.result();
        _result->fermata->setParent(_result.get());
    } else if (strcmp(qname.localName(), kOrnamentsTag) == 0) {
        auto ornament = _ornamentsHandler.result();
        ornament->setParent(_result.get());
        _result->ornaments.push_back(std::move(ornament));
    } else if (strcmp(qname.localName(), kSlurTag) == 0) {
        auto slur = _slurHandler.result();
        slur->setParent(_result.get());
        _result->slurs.push_back(std::move(slur));
    } else if (strcmp(qname.localName(), kTiedTag) == 0) {
        auto tie = _tiedHandler.result();
        tie->setParent(_result.get());
        _result->ties.push_back(std::move(tie));
    } else if (strcmp(qname.localName(), kTupletTag) == 0) {
        auto tuplet = _tupletHandler.result();
        tuplet->setParent(_result.get());
        _result->tuplets.push_back(std::move(tuplet));
    }
}

} // namsepace parsing
} // namespace mxml
