// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once
#include <mxml/dom/Attributes.h>
#include <mxml/dom/Chord.h>
#include <mxml/dom/Score.h>
#include <mxml/dom/Note.h>
#include <mxml/dom/Ornaments.h>
#include <mxml/dom/Types.h>
#include <memory>


namespace mxml {

class ScoreBuilder {
public:
    ScoreBuilder();

    dom::Part* addPart();
    dom::Measure* addMeasure(dom::Part* part);

    dom::Attributes* addAttributes(dom::Measure* measure);
    dom::Key* setKey(dom::Attributes* attributes, int staff = 1);
    dom::Time* setTime(dom::Attributes* attributes);
    dom::Clef* setTrebleClef(dom::Attributes* attributes, int staff = 1);
    dom::Clef* setBassClef(dom::Attributes* attributes, int staff = 2);

    dom::Chord* addChord(dom::Measure* measure);
    dom::Note* addNote(dom::Chord* chord, dom::Note::Type type = dom::Note::Type::Eighth, dom::time_t start = 0, dom::time_t duration = 1);
    dom::Note* addNote(dom::Measure* measure, dom::Note::Type type = dom::Note::Type::Eighth, dom::time_t start = 0, dom::time_t duration = 1);
    dom::Pitch* setPitch(dom::Note* note, dom::Pitch::Step step, int octave, int alter = 0);
    
    dom::Ornaments* addTrill(dom::Note* note, dom::Placement placement);
    dom::Ornaments* addInvertedMordent(dom::Note* note, bool isLong);
    dom::Ornaments* addMordent(dom::Note* note, bool isLong);
    dom::Ornaments* addInvertedTurn(dom::Note* note, bool slash);
    dom::Ornaments* addTurn(dom::Note* note, bool slash);

    std::unique_ptr<dom::Score> build();

private:
    std::unique_ptr<dom::Score> _score;
};

}
