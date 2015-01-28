//  Created by Alejandro Isaza on 2015-01-13.
//  Copyright (c) 2015 Venture Media Labs. All rights reserved.

#pragma once
#include <mxml/dom/Attributes.h>
#include <mxml/dom/Chord.h>
#include <mxml/dom/Score.h>
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
    dom::Note* addNote(dom::Chord* chord, dom::Note::Type type = dom::Note::TYPE_EIGHTH, dom::time_t start = 0, dom::time_t duration = 1);
    dom::Pitch* setPitch(dom::Note* note, dom::Pitch::Step step, int octave, dom::Optional<int> alter = 0);

    std::unique_ptr<dom::Score> build();

private:
    std::unique_ptr<dom::Score> _score;
};

}
