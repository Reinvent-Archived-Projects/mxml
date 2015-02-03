//  Created by Alejandro Isaza on 2015-01-13.
//  Copyright (c) 2015 Venture Media Labs. All rights reserved.

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

dom::Pitch* ScoreBuilder::setPitch(dom::Note* note, dom::Pitch::Step step, int octave, dom::Optional<int> alter) {
    auto pitch = std::unique_ptr<dom::Pitch>(new dom::Pitch{});
    pitch->setParent(note);
    pitch->setOctave(octave);
    pitch->setStep(step);
    pitch->setAlter(alter);

    note->setPitch(std::move(pitch));
    return note->pitch().get();
}

std::unique_ptr<dom::Score> ScoreBuilder::build() {
    return std::move(_score);
}

}
