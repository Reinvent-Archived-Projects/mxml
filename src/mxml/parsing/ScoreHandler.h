// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once
#include <lxml/BaseRecursiveHandler.h>
#include "CreditHandler.h"
#include "DefaultsHandler.h"
#include "IdentificationHandler.h"
#include "PartHandler.h"
#include <mxml/dom/Score.h>

#include <memory>

namespace mxml {
namespace parsing {

class ScoreHandler : public lxml::BaseRecursiveHandler<std::unique_ptr<dom::Score>> {
public:
    void startElement(const lxml::QName& qname, const AttributeMap& attributes);
    RecursiveHandler* startSubElement(const lxml::QName& qname);
    void endSubElement(const lxml::QName& qname, lxml::RecursiveHandler* parser);
    
private:
    IdentificationHandler _identificationHandler;
    CreditHandler _creditHandler;
    DefaultsHandler _defaultsHandler;
    PartHandler _partHandler;
    std::size_t _partIndex;
};

} // namespace parsing
} // namespace mxml
