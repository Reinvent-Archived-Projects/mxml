//  Created by Alejandro Isaza on 2/20/2014.
//  Copyright (c) 2014 Venture Media Labs Inc. All rights reserved.

#include "ScoreHandler.h"
#include <cstring>

using namespace lxml;

namespace mxml {

using dom::Score;
using lxml::QName;

static const char* kIdentificationTag = "identification";
static const char* kDefaultsTag = "defaults";
static const char* kCreditTag = "credit";
static const char* kPartTag = "part";

void ScoreHandler::startElement(const QName& qname, const AttributeMap& attributes) {
    _result.reset(new Score());
    _partIndex = 0;
}

RecursiveHandler* ScoreHandler::startSubElement(const QName& qname) {
    if (strcmp(qname.localName(), kIdentificationTag) == 0)
        return &_identificationHandler;
    else if (strcmp(qname.localName(), kDefaultsTag) == 0)
        return &_defaultsHandler;
    else if (strcmp(qname.localName(), kCreditTag) == 0)
        return &_creditHandler;
    else if (strcmp(qname.localName(), kPartTag) == 0)
        return &_partHandler;
    return 0;
}

void ScoreHandler::endSubElement(const QName& qname, RecursiveHandler* parser) {
    if (strcmp(qname.localName(), kIdentificationTag) == 0)
        _result->setIdentification(_identificationHandler.result());
    else if (strcmp(qname.localName(), kDefaultsTag) == 0)
        _result->setDefaults(_defaultsHandler.result());
    else if (strcmp(qname.localName(), kCreditTag) == 0)
        _result->addCredit(_creditHandler.result());
    else if (strcmp(qname.localName(), kPartTag) == 0) {
        auto part = _partHandler.result();
        part->setParent(_result.get());
        part->setIndex(_partIndex++);
        _result->addPart(std::move(part));
    }
}

} // namespace mxml
