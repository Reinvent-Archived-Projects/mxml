//  Created by Alejandro Isaza on 2/20/2014.
//  Copyright (c) 2014 Venture Media Labs Inc. All rights reserved.

#include "NoteHandler.h"
#include <mxml/dom/InvalidDataError.h>
#include <cstring>

namespace mxml {

using dom::Note;
using lxml::QName;

static const char* kDefaultXAttribute = "default-x";
static const char* kDefaultYAttribute = "default-y";
static const char* kPrintObjectAttribute = "print-object";
static const char* kDynamicsAttribute = "dynamics";
static const char* kEndDynamicsAttribute = "end-dynamics";
static const char* kAttackAttribute = "attack";
static const char* kReleaseAttribute = "release";

static const char* kDurationTag = "duration";
static const char* kTypeTag = "type";
static const char* kChordTag = "chord";
static const char* kGraceTag = "grace";
static const char* kStemTag = "stem";
static const char* kStaffTag = "staff";
static const char* kPitchTag = "pitch";
static const char* kRestTag = "rest";
static const char* kUnpitchedTag = "unpitched";
static const char* kAccidentalTag = "accidental";
static const char* kDotTag = "dot";
static const char* kTieTag = "tie";
static const char* kNotationsTag = "notations";
static const char* kBeamTag = "beam";
static const char* kLyricTag = "lyric";

void NoteHandler::startElement(const QName& qname, const AttributeMap& attributes) {
    using dom::presentOptional;
    using lxml::DoubleHandler;
    using lxml::IntegerHandler;

    _result.reset(new Note());
    
    auto defaultX = attributes.find(kDefaultXAttribute);
    if (defaultX != attributes.end())
        _result->setDefaultX(presentOptional((float)DoubleHandler::parseDouble(defaultX->second)));
    
    auto defaultY = attributes.find(kDefaultYAttribute);
    if (defaultY != attributes.end())
        _result->setDefaultY(presentOptional((float)DoubleHandler::parseDouble(defaultY->second)));
    
    auto printObject = attributes.find(kPrintObjectAttribute);
    if (printObject != attributes.end())
        _result->setPrintObject(printObject->second == "yes" ? true : false);

    auto dynamics = attributes.find(kDynamicsAttribute);
    if (dynamics != attributes.end())
        _result->setDynamics(presentOptional((float)DoubleHandler::parseDouble(dynamics->second)));

    auto endDynamics = attributes.find(kEndDynamicsAttribute);
    if (endDynamics != attributes.end())
        _result->setEndDynamics(presentOptional((float)DoubleHandler::parseDouble(endDynamics->second)));

    auto attack = attributes.find(kAttackAttribute);
    if (attack != attributes.end())
        _result->setAttack(IntegerHandler::parseInteger(attack->second));

    auto release = attributes.find(kReleaseAttribute);
    if (release != attributes.end())
        _result->setRelease(IntegerHandler::parseInteger(release->second));
}

void NoteHandler::endElement(const lxml::QName& qname, const std::string& contents) {
    auto& pitch = _result->pitch();
    if (pitch) {
        // Avoid alter values outside of the range [-2..2]
        if (pitch->alter() >= 3) {
            while (pitch->alter() > 1)
                increasePitch(*pitch);
        }
        if (pitch->alter() <= -3) {
            while (pitch->alter() < -1)
                decreasePitch(*pitch);
        }
    }
}

lxml::RecursiveHandler* NoteHandler::startSubElement(const QName& qname) {
    if (strcmp(qname.localName(), kDurationTag) == 0)
        return &_integerHandler;
    else if (strcmp(qname.localName(), kTypeTag) == 0)
        return &_stringHandler;
    else if (strcmp(qname.localName(), kChordTag) == 0)
        return &_presenceHandler;
    else if (strcmp(qname.localName(), kGraceTag) == 0)
        return &_presenceHandler;
    else if (strcmp(qname.localName(), kStemTag) == 0)
        return &_stringHandler;
    else if (strcmp(qname.localName(), kStaffTag) == 0)
        return &_integerHandler;
    else if (strcmp(qname.localName(), kPitchTag) == 0)
        return &_pitchHandler;
    else if (strcmp(qname.localName(), kRestTag) == 0)
        return &_restHandler;
    else if (strcmp(qname.localName(), kUnpitchedTag) == 0)
        return &_unpitchedHandler;
    else if (strcmp(qname.localName(), kAccidentalTag) == 0)
        return &_stringHandler;
    else if (strcmp(qname.localName(), kDotTag) == 0)
        return &_emptyPlacementHandler;
    else if (strcmp(qname.localName(), kTieTag) == 0)
        return &_tieHandler;
    else if (strcmp(qname.localName(), kNotationsTag) == 0)
        return &_notationsHandler;
    else if (strcmp(qname.localName(), kBeamTag) == 0)
        return &_beamHandler;
    else if (strcmp(qname.localName(), kLyricTag) == 0)
        return &_lyricHandler;
    return 0;
}

void NoteHandler::endSubElement(const QName& qname, RecursiveHandler* parser) {
    using dom::presentOptional;

    if (strcmp(qname.localName(), kDurationTag) == 0)
        _result->setDuration(presentOptional(_integerHandler.result()));
    else if (strcmp(qname.localName(), kTypeTag) == 0)
        _result->setType(presentOptional(typeFromString(_stringHandler.result())));
    else if (strcmp(qname.localName(), kChordTag) == 0)
        _result->setChord(_presenceHandler.result());
    else if (strcmp(qname.localName(), kGraceTag) == 0)
        _result->setGrace(_presenceHandler.result());
    else if (strcmp(qname.localName(), kStemTag) == 0)
        _result->setStem(stemFromString(_stringHandler.result()));
    else if (strcmp(qname.localName(), kStaffTag) == 0)
        _result->setStaff(_integerHandler.result());
    else if (strcmp(qname.localName(), kPitchTag) == 0)
        _result->setPitch(_pitchHandler.result());
    else if (strcmp(qname.localName(), kRestTag) == 0)
        _result->setRest(_restHandler.result());
    else if (strcmp(qname.localName(), kUnpitchedTag) == 0)
        _result->setUnpitched(_unpitchedHandler.result());
    else if (strcmp(qname.localName(), kAccidentalTag) == 0) {
        std::unique_ptr<dom::Accidental> accidental(new dom::Accidental(accidentalTypeFromString(_stringHandler.result())));
        _result->setAccidental(std::move(accidental));
    }
    else if (strcmp(qname.localName(), kDotTag) == 0)
        _result->setDot(_emptyPlacementHandler.result());
    else if (strcmp(qname.localName(), kTieTag) == 0)
        _result->setTie(_tieHandler.result());
    else if (strcmp(qname.localName(), kNotationsTag) == 0) {
        _result->setNotations(_notationsHandler.result());
    } else if (strcmp(qname.localName(), kBeamTag) == 0)
        _result->addBeam(_beamHandler.result());
    else if (strcmp(qname.localName(), kLyricTag) == 0)
        _result->addLyric(_lyricHandler.result());
}

Note::Type NoteHandler::typeFromString(const std::string& string) {
    if (string == "1024th") return Note::TYPE_1024TH;
    if (string == "512th") return Note::TYPE_512TH;
    if (string == "256th") return Note::TYPE_256TH;
    if (string == "128th") return Note::TYPE_128TH;
    if (string == "64th") return Note::TYPE_64TH;
    if (string == "32nd") return Note::TYPE_32ND;
    if (string == "16th") return Note::TYPE_16TH;
    if (string == "eighth") return Note::TYPE_EIGHTH;
    if (string == "quarter") return Note::TYPE_QUARTER;
    if (string == "half") return Note::TYPE_HALF;
    if (string == "whole") return Note::TYPE_WHOLE;
    if (string == "breve") return Note::TYPE_BREVE;
    if (string == "long") return Note::TYPE_LONG;
    if (string == "maxima") return Note::TYPE_MAXIMA;
    throw dom::InvalidDataError("Invalid note type " + string);
}

dom::Stem NoteHandler::stemFromString(const std::string& string) {
    if (string == "none") return dom::STEM_NONE;
    if (string == "up") return dom::STEM_UP;
    if (string == "down") return dom::STEM_DOWN;
    if (string == "double") return dom::STEM_DOUBLE;
    throw dom::InvalidDataError("Invalid note stem " + string);
}

dom::Accidental::Type NoteHandler::accidentalTypeFromString(const std::string& string) {
    if (string == "sharp") return dom::Accidental::TYPE_SHARP;
    if (string == "flat") return dom::Accidental::TYPE_FLAT;
    if (string == "natural") return dom::Accidental::TYPE_NATURAL;
    if (string == "double-sharp" || string == "sharp-sharp") return dom::Accidental::TYPE_DOUBLE_SHARP;
    if (string == "flat-flat") return dom::Accidental::TYPE_DOUBLE_FLAT;
    throw dom::InvalidDataError("Invalid accidental type " + string);
}

void NoteHandler::increasePitch(dom::Pitch& pitch) {
    auto octave = pitch.octave();
    auto step = pitch.step();
    auto alter = pitch.alter();

    switch (step) {
        case dom::Pitch::STEP_C:
        case dom::Pitch::STEP_D:
        case dom::Pitch::STEP_F:
        case dom::Pitch::STEP_G:
        case dom::Pitch::STEP_A:
            step = static_cast<dom::Pitch::Step>(step + 1);
            alter -= 2;
            break;

        case dom::Pitch::STEP_E:
        case dom::Pitch::STEP_B:
            step = static_cast<dom::Pitch::Step>(step + 1);
            alter -= 1;
            break;

        default:
            break;
    }

    if (step >= dom::Pitch::kStepCount) {
        octave += 1;
        step = static_cast<dom::Pitch::Step>(step - dom::Pitch::kStepCount);
    }

    pitch.setOctave(octave);
    pitch.setStep(step);
    pitch.setAlter(dom::presentOptional(alter));
}

void NoteHandler::decreasePitch(dom::Pitch& pitch) {
    auto octave = pitch.octave();
    auto step = pitch.step();
    auto alter = pitch.alter();

    switch (step) {
        case dom::Pitch::STEP_D:
        case dom::Pitch::STEP_E:
        case dom::Pitch::STEP_G:
        case dom::Pitch::STEP_A:
        case dom::Pitch::STEP_B:
            step = static_cast<dom::Pitch::Step>(step - 1);
            alter += 2;
            break;

        case dom::Pitch::STEP_C:
        case dom::Pitch::STEP_F:
            step = static_cast<dom::Pitch::Step>(step -1);
            alter += 1;
            break;

        default:
            break;
    }
    
    if (step < 0) {
        octave -= 1;
        step = static_cast<dom::Pitch::Step>(step + dom::Pitch::kStepCount);
    }
    
    pitch.setOctave(octave);
    pitch.setStep(step);
    pitch.setAlter(dom::presentOptional(alter));
}

} // namespace mxml
