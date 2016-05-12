// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "ScoreBuilder.h"

namespace mxml {

ScoreBuilder::ScoreBuilder() : _score(new dom::Score{}) {
}

dom::Part* ScoreBuilder::addPart() {
    auto part = std::unique_ptr<dom::Part>(new dom::Part{});
    part->setParent(_score.get());
    part->setIndex(_score->parts().size());
    _score->addPart(std::move(part));
    return _score->parts().back().get();
}

dom::Measure* ScoreBuilder::addMeasure(dom::Part* part) {
    auto measure = std::unique_ptr<dom::Measure>(new dom::Measure{});
    measure->setParent(part);
    measure->setIndex(part->measures().size());
    part->addMeasure(std::move(measure));
    return part->measures().back().get();
}

dom::Attributes* ScoreBuilder::addAttributes(dom::Measure* measure) {
    auto attributes = std::unique_ptr<dom::Attributes>(new dom::Attributes{});
    auto raw = attributes.get();
    attributes->setParent(measure);
    measure->addNode(std::move(attributes));
    return raw;
}

dom::Key* ScoreBuilder::setKey(dom::Attributes* attributes, int staff) {
    auto key = std::unique_ptr<dom::Key>(new dom::Key{});
    key->setParent(attributes);
    attributes->setKey(staff, std::move(key));
    return attributes->key(staff);
}

dom::Time* ScoreBuilder::setTime(dom::Attributes* attributes) {
    auto time = std::unique_ptr<dom::Time>(new dom::Time{});
    time->setParent(attributes);
    attributes->setTime(std::move(time));
    return attributes->time();
}

dom::Clef* ScoreBuilder::setTrebleClef(dom::Attributes* attributes, int staff) {
    auto clef = dom::Clef::trebleClef(staff);
    clef->setParent(attributes);
    attributes->setClef(staff, std::move(clef));
    return attributes->clef(staff);
}

dom::Clef* ScoreBuilder::setBassClef(dom::Attributes* attributes, int staff) {
    auto clef = dom::Clef::bassClef(staff);
    clef->setParent(attributes);
    attributes->setClef(staff, std::move(clef));
    return attributes->clef(staff);
}

dom::Chord* ScoreBuilder::addChord(dom::Measure* measure) {
    auto chord = std::unique_ptr<dom::Chord>(new dom::Chord{});
    auto raw = chord.get();
    chord->setParent(measure);
    measure->addNode(std::move(chord));
    return raw;
}

dom::Note* ScoreBuilder::addNote(dom::Chord* chord, dom::Note::Type type, dom::time_t start, dom::time_t duration) {
    auto note = std::unique_ptr<dom::Note>(new dom::Note{});
    note->setParent(chord);
    note->setMeasure(static_cast<const dom::Measure*>(chord->parent()));
    note->setType(dom::presentOptional(type));
    note->setStart(start);
    note->setDuration(dom::presentOptional(duration));

    chord->addNote(std::move(note));
    return chord->notes().back().get();
}

dom::Note* ScoreBuilder::addNote(dom::Measure* measure, dom::Note::Type type, dom::time_t start, dom::time_t duration) {
    auto chord = addChord(measure);
    return addNote(chord, type, start, duration);
}

dom::Pitch* ScoreBuilder::setPitch(dom::Note* note, dom::Pitch::Step step, int octave, int alter) {
    auto pitch = std::unique_ptr<dom::Pitch>(new dom::Pitch{});
    pitch->setParent(note);
    pitch->setOctave(octave);
    pitch->setStep(step);
    pitch->setAlter(alter);

    note->pitch = std::move(pitch);
    return note->pitch.get();
}
    
dom::Ornaments* ScoreBuilder::addTrill(dom::Note* note, dom::Placement placement) {
    auto ornament = std::unique_ptr<dom::Ornaments>(new dom::Ornaments());
    auto emptyPlacement = std::unique_ptr<dom::EmptyPlacement>(new dom::EmptyPlacement());
    emptyPlacement->setPlacement(placement);
    ornament->setTrillMark(std::move(emptyPlacement));
    
    if (!note->notations)
        note->notations.reset(new dom::Notations());
    
    note->notations->ornaments.push_back(std::move(ornament));
    return note->notations->ornaments.back().get();
}

dom::Ornaments* ScoreBuilder::addInvertedMordent(dom::Note* note, bool isLong) {
    auto ornament = std::unique_ptr<dom::Ornaments>(new dom::Ornaments());
    auto mordent = std::unique_ptr<dom::Mordent>(new dom::Mordent());
    mordent->setLong(isLong);
    ornament->setInvertedMordent(std::move(mordent));
    
    if (!note->notations)
        note->notations.reset(new dom::Notations());
    
    note->notations->ornaments.push_back(std::move(ornament));
    return note->notations->ornaments.back().get();
}

dom::Ornaments* ScoreBuilder::addMordent(dom::Note* note, bool isLong) {
    auto ornament = std::unique_ptr<dom::Ornaments>(new dom::Ornaments());
    auto mordent = std::unique_ptr<dom::Mordent>(new dom::Mordent());
    mordent->setLong(isLong);
    ornament->setMordent(std::move(mordent));
    
    if (!note->notations)
        note->notations.reset(new dom::Notations());
    
    note->notations->ornaments.push_back(std::move(ornament));
    return note->notations->ornaments.back().get();
}

dom::Ornaments* ScoreBuilder::addInvertedTurn(dom::Note* note, bool slash) {
    auto ornament = std::unique_ptr<dom::Ornaments>(new dom::Ornaments());
    auto turn = std::unique_ptr<dom::Turn>(new dom::Turn());
    turn->setSlash(slash);
    ornament->setInvertedTurn(std::move(turn));
    
    if (!note->notations)
        note->notations.reset(new dom::Notations());
    
    note->notations->ornaments.push_back(std::move(ornament));
    return note->notations->ornaments.back().get();
}

dom::Ornaments* ScoreBuilder::addTurn(dom::Note* note, bool slash) {
    auto ornament = std::unique_ptr<dom::Ornaments>(new dom::Ornaments());
    auto turn = std::unique_ptr<dom::Turn>(new dom::Turn());
    turn->setSlash(slash);
    ornament->setTurn(std::move(turn));
    
    if (!note->notations)
        note->notations.reset(new dom::Notations());
    
    note->notations->ornaments.push_back(std::move(ornament));
    return note->notations->ornaments.back().get();
}

std::unique_ptr<dom::Score> ScoreBuilder::build() {
    return std::move(_score);
}

}
