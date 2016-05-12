// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Metrics.h"

namespace mxml {

using namespace dom;
    
const dom::tenths_t Metrics::kStaffLineSpacing = 10;
const std::size_t Metrics::kStaffLineCount = 5;

Metrics::Metrics(const dom::Score& score, const ScoreProperties& scoreProperties, std::size_t partIndex)
: _score(score),
  _scoreProperties(scoreProperties),
  _prints(),
  _systemCount(0),
  _pageCount(0),
  _partIndex(partIndex)
{
    auto& part = score.parts().at(partIndex);
    for (auto& measure : part->measures())
        process(*measure);
}

void Metrics::process(const dom::Measure& measure) {
    auto measureIndex = measure.index();
    for (auto& node : measure.nodes()) {
        if (auto print = dynamic_cast<const dom::Print*>(node.get()))
            process(measureIndex, *print);
    }
}

void Metrics::process(std::size_t measureIndex, const dom::Print& print) {
    if (print.newSystem && measureIndex != 0)
        _systemCount += 1;
    if (print.newPage && measureIndex != 0)
        _pageCount += 1;

    PrintRef ref;
    ref.systemIndex = _systemCount;
    ref.measureIndex = measureIndex;
    ref.print = &print;
    _prints.insert(ref);
}

std::size_t Metrics::staves() const {
    return _scoreProperties.staves(_partIndex);
}

dom::tenths_t Metrics::staffHeight() {
    return (kStaffLineCount - 1) * kStaffLineSpacing;
}

dom::tenths_t Metrics::stavesHeight() const {
    return stavesHeight(staves(), staffDistance());
}

dom::tenths_t Metrics::stavesHeight(std::size_t staves, dom::tenths_t staffDistance) {
    return (Metrics::staffHeight() + staffDistance) * staves - staffDistance;
}

dom::tenths_t Metrics::staffOrigin(int staffNumber) const {
    return (staffNumber - 1) * (staffHeight() + staffDistance());
}

dom::tenths_t Metrics::noteY(const dom::Note& note) const {
    return staffOrigin(note.staff()) + staffY(note);
}

dom::tenths_t Metrics::staffY(const Note& note) const {
    auto& clef = *_scoreProperties.clef(note);

    dom::tenths_t y = 20;
    if (note.pitch) {
        y = staffY(clef, *note.pitch);
    } else if (note.rest) {
        y = staffY(clef, *note.rest);
    } else if (note.position.defaultY.isPresent()) {
        y = note.position.defaultY;
    }

    auto measure = note.measure();
    auto part = measure->part();
    int shift = _scoreProperties.octaveShift(part->index(), measure->index(), note.staff(), note.start());
    
    return -shift * kStaffLineSpacing/2 + y * kStaffLineSpacing / 10;
}

dom::tenths_t Metrics::staffY(const Clef& clef, const Pitch& pitch) {
    switch (clef.sign().value()) {
        case Clef::Sign::G: return staffYInGClef(pitch.step(), pitch.octave());
        case Clef::Sign::F: return staffYInFClef(pitch.step(), pitch.octave());
        case Clef::Sign::C: return staffYInCClef(pitch.step(), pitch.octave());
        default: return 20;
    }
}

dom::tenths_t Metrics::staffY(const Clef& clef, const Rest& rest) {
    if (!rest.displayStep().isPresent() || !rest.displayOctave().isPresent())
        return 20;
    
    switch (clef.sign().value()) {
        case Clef::Sign::G: return staffYInGClef(rest.displayStep(), rest.displayOctave());
        case Clef::Sign::F: return staffYInFClef(rest.displayStep(), rest.displayOctave());
        case Clef::Sign::C: return staffYInCClef(rest.displayStep(), rest.displayOctave());
        default: return 20;
    }
}

dom::tenths_t Metrics::staffYInGClef(Pitch::Step step, int octave) {
    dom::tenths_t y;
    switch (step) {
        case Pitch::Step::C: y = 50; break;
        case Pitch::Step::D: y = 45; break;
        case Pitch::Step::E: y = 40; break;
        case Pitch::Step::F: y = 35; break;
        case Pitch::Step::G: y = 30; break;
        case Pitch::Step::A: y = 25; break;
        case Pitch::Step::B: y = 20; break;
        default: y = 20;
    }
    return y - (octave - 4) * 35;
}

dom::tenths_t Metrics::staffYInCClef(Pitch::Step step, int octave) {
    dom::tenths_t y;
    switch (step) {
        case Pitch::Step::C: y = 20; break;
        case Pitch::Step::D: y = 15; break;
        case Pitch::Step::E: y = 10; break;
        case Pitch::Step::F: y = 5; break;
        case Pitch::Step::G: y = 0; break;
        case Pitch::Step::A: y = -5; break;
        case Pitch::Step::B: y = -10; break;
        default: y = 20;
    }
    return y - (octave - 5) * 35;
}

dom::tenths_t Metrics::staffYInFClef(Pitch::Step step, int octave) {
    dom::tenths_t y;
    switch (step) {
        case Pitch::Step::C: y = 25; break;
        case Pitch::Step::D: y = 20; break;
        case Pitch::Step::E: y = 15; break;
        case Pitch::Step::F: y = 10; break;
        case Pitch::Step::G: y = 5; break;
        case Pitch::Step::A: y = 0; break;
        case Pitch::Step::B: y = -5; break;
        default: y = 20;
    }
    return y - (octave - 3) * 35;
}

}
