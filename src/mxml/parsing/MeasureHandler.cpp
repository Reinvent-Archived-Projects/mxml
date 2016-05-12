// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "MeasureHandler.h"
#include <mxml/dom/Chord.h>
#include <cstring>

namespace mxml {
namespace parsing {

using dom::Measure;
using lxml::QName;

static const char* kNoteTag = "note";
static const char* kBackupTag = "backup";
static const char* kForwardTag = "forward";
static const char* kAttributesTag = "attributes";
static const char* kDirectionTag = "direction";
static const char* kPrintTag = "print";
static const char* kBarlineTag = "barline";

static const char* kNumberAttribute = "number";

void MeasureHandler::startElement(const QName& qname, const AttributeMap& attributes) {
    endChord();
    
    _result.reset(new Measure());
    _time = 0;
    _empty = true;
    
    auto numberit = attributes.find(kNumberAttribute);
    if (numberit != attributes.end())
        _result->setNumber(numberit->second);
}

void MeasureHandler::endElement(const QName& qname, const std::string& contents) {
    if (_empty) {
        auto note = std::unique_ptr<dom::Note>(new dom::Note);
        note->rest = std::unique_ptr<dom::Rest>(new dom::Rest);
        note->setMeasure(_result.get());
        note->setStart(0);
        note->setType(dom::presentOptional(dom::Note::Type::Whole));
        //note->setDuration(dom::presentOptional(dom::Attributes::divisionsPerMeasure(_attributes->divisions(), *_attributes->time())));
        _result->addNode(std::move(note));
    }
}

lxml::RecursiveHandler* MeasureHandler::startSubElement(const QName& qname) {
    if (strcmp(qname.localName(), kNoteTag) == 0)
        return &_noteHandler;
    else if (strcmp(qname.localName(), kBackupTag) == 0)
        return &_backupHandler;
    else if (strcmp(qname.localName(), kForwardTag) == 0)
        return &_forwardHandler;
    else if (strcmp(qname.localName(), kAttributesTag) == 0)
        return &_attributesHandler;
    else if (strcmp(qname.localName(), kDirectionTag) == 0)
        return &_directionHandler;
    else if (strcmp(qname.localName(), kPrintTag) == 0)
        return &_printHandler;
    else if (strcmp(qname.localName(), kBarlineTag) == 0)
        return &_barlineHandler;
    return 0;
}

void MeasureHandler::endSubElement(const QName& qname, RecursiveHandler* parser) {
    if (strcmp(qname.localName(), kNoteTag) == 0) {
        handleNote(_noteHandler.result());
    } else if(strcmp(qname.localName(), kForwardTag) == 0) {
        std::unique_ptr<dom::Forward> forward = _forwardHandler.result();
        _time += forward->duration();
        _result->addNode(std::move(forward));
    } else if(strcmp(qname.localName(), kBackupTag) == 0) {
        std::unique_ptr<dom::Backup> backup = _backupHandler.result();
        _time -= backup->duration();
        _result->addNode(std::move(backup));
    } else if (strcmp(qname.localName(), kAttributesTag) == 0) {
        auto attributes = _attributesHandler.result();
        attributes->setStart(_time);
        attributes->setParent(_result.get());
        _result->addNode(std::move(attributes));
    } else if (strcmp(qname.localName(), kDirectionTag) == 0) {
        std::unique_ptr<dom::Direction> direction = _directionHandler.result();
        direction->setStart(_time);
        _result->addNode(std::move(direction));
    } else if (strcmp(qname.localName(), kPrintTag) == 0) {
        _result->addNode(_printHandler.result());
    } else if (strcmp(qname.localName(), kBarlineTag) == 0) {
        _result->addNode(_barlineHandler.result());
    }
}

void MeasureHandler::handleNote(std::unique_ptr<dom::Note>&& note) {
    _empty = false;

    note->setMeasure(_result.get());
    note->setStart(_time);
    
    // Only the first note of a chord advances the time
    if (!note->chord()) {
        _lastTime = _time;
        _time += note->duration();
    }
    
    // Rests are a special case. A rest ends a chord but can appear whitin a beamed set.
    if (note->rest) {
        endChord();
        _result->addNode(std::move(note));
        return;
    }
    
    // Either continue or start a chord
    if (!_chord || !note->chord()) {
        endChord();
        startChord(std::move(note));
    } else {
        note->setStart(_lastTime);
        _chord->addNote(std::move(note));
    }
}

void MeasureHandler::startChord(std::unique_ptr<dom::Note>&& note) {
    std::unique_ptr<dom::Chord> chord(new dom::Chord);
    _chord = chord.get();
    _chord->addNote(std::move(note));
    _result->addNode(std::move(chord));
}

void MeasureHandler::endChord() {
    _chord = 0;
}

} // namespace parsing
} // namespace mxml
