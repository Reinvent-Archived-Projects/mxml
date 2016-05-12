// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once
#include <lxml/BaseRecursiveHandler.h>
#include <lxml/IntegerHandler.h>
#include <lxml/PresenceHandler.h>
#include <lxml/StringHandler.h>

#include <mxml/dom/Note.h>
#include <memory>

#include "BeamHandler.h"
#include "EmptyPlacementHandler.h"
#include "LyricHandler.h"
#include "NotationsHandler.h"
#include "PitchHandler.h"
#include "RestHandler.h"
#include "TieHandler.h"
#include "TimeModificationHandler.h"
#include "UnpitchedHandler.h"


namespace mxml {
namespace parsing {

class NoteHandler : public lxml::BaseRecursiveHandler<std::unique_ptr<dom::Note>> {
public:
    void startElement(const lxml::QName& qname, const AttributeMap& attributes);
    void endElement(const lxml::QName& qname, const std::string& contents);

    RecursiveHandler* startSubElement(const lxml::QName& qname);
    void endSubElement(const lxml::QName& qname, lxml::RecursiveHandler* parser);
    
    static dom::Note::Type typeFromString(const std::string& string);
    static dom::Stem stemFromString(const std::string& string);
    static dom::Accidental::Type accidentalTypeFromString(const std::string& string);

protected:
    /**
     Increase the pitch to avoid having alter values lower than -2
     */
    void increasePitch(dom::Pitch& pitch);

    /**
     Decrease the pitch to avoid having alter values greater than 2
     */
    void decreasePitch(dom::Pitch& pitch);

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
    TimeModificationHandler _timeModificationHandler;
};

} // namespace parsing
} // namespace mxml
