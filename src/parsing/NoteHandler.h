//  Created by Alejandro Isaza on 2/20/2014.
//  Copyright (c) 2014 Venture Media Labs Inc. All rights reserved.

#pragma once
#include "BaseRecursiveHandler.h"
#include "BeamHandler.h"
#include "EmptyPlacementHandler.h"
#include "IntegerHandler.h"
#include "LyricHandler.h"
#include "NotationsHandler.h"
#include "PitchHandler.h"
#include "PresenceHandler.h"
#include "RestHandler.h"
#include "StringHandler.h"
#include "TieHandler.h"
#include "UnpitchedHandler.h"

#include "dom/Note.h"
#include <memory>

namespace mxml {

class NoteHandler : public lxml::BaseRecursiveHandler<std::unique_ptr<dom::Note>> {
public:
    void startElement(const lxml::QName& qname, const AttributeMap& attributes);
    RecursiveHandler* startSubElement(const lxml::QName& qname);
    void endSubElement(const lxml::QName& qname, lxml::RecursiveHandler* parser);
    
    static dom::Note::Type typeFromString(const std::string& string);
    static dom::Stem stemFromString(const std::string& string);
    static dom::Accidental::Type accidentalTypeFromString(const std::string& string);
    
private:
    lxml::IntegerHandler _integerHandler;
    lxml::StringHandler _stringHandler;
    lxml::PresenceHandler _presenceHandler;

    PitchHandler _pitchHandler;
    RestHandler _restHandler;
    UnpitchedHandler _unpitchedHandler;
    BeamHandler _beamHandler;
    EmptyPlacementHandler _emptyPlacementHandler;
    TieHandler _tieHandler;
    NotationsHandler _notationsHandler;
    LyricHandler _lyricHandler;
};

} // namespace mxml
