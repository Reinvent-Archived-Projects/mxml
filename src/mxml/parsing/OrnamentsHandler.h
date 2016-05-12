// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once
#include <lxml/BaseRecursiveHandler.h>
#include "EmptyPlacementHandler.h"
#include "MordentHandler.h"
#include "TurnHandler.h"

#include <mxml/dom/Ornaments.h>

namespace mxml {

class OrnamentsHandler : public lxml::BaseRecursiveHandler<std::unique_ptr<dom::Ornaments>> {
public:
    void startElement(const lxml::QName& qname, const AttributeMap& attributes);
    RecursiveHandler* startSubElement(const lxml::QName& qname);
    void endSubElement(const lxml::QName& qname, RecursiveHandler* parser);
    
private:
    EmptyPlacementHandler _emptyPlacementHandler;
    MordentHandler _mordentHandler;
    TurnHandler _turnHandler;
};

} // namespace
