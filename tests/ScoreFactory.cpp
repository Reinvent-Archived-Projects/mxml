//  Created by Alejandro Isaza on 2015-01-12.
//  Copyright (c) 2015 Venture Media Labs. All rights reserved.

#include "ScoreFactory.h"

#include <mxml/dom/Attributes.h>


namespace mxml {
namespace tests {

    std::unique_ptr<dom::Score> ScoreFactory::buildScore() {
        auto score = std::unique_ptr<dom::Score>(new dom::Score{});

        auto part = std::unique_ptr<dom::Part>(new dom::Part{});
        part->setParent(score.get());
        part->setIndex(0);

        score->addPart(std::move(part));
        
        return score;

    }

    std::unique_ptr<dom::Score> ScoreFactory::buildScoreWithAttributes() {
        auto score = buildScore();
        auto& part = score->parts().at(0);

        auto measure = std::unique_ptr<dom::Measure>(new dom::Measure{});
        measure->setParent(part.get());
        measure->setIndex(0);

        auto attributes = std::unique_ptr<dom::Attributes>(new dom::Attributes{});
        attributes->setParent(measure.get());
        attributes->setStaves(dom::presentOptional(1));
        attributes->setStart(0);
        attributes->setDivisions(dom::presentOptional(8));

        measure->addNode(std::move(attributes));
        part->addMeasure(std::move(measure));
        
        return score;
    }

    std::unique_ptr<dom::Score> ScoreFactory::buildScorePiano() {
        auto score = buildScore();
        auto& part = score->parts().at(0);

        auto measure = std::unique_ptr<dom::Measure>(new dom::Measure{});
        measure->setParent(part.get());
        measure->setIndex(0);

        auto attributes = std::unique_ptr<dom::Attributes>(new dom::Attributes{});
        attributes->setParent(measure.get());
        attributes->setStaves(dom::presentOptional(2));
        attributes->setStart(0);
        attributes->setDivisions(dom::presentOptional(8));
        attributes->setClef(1, dom::Clef::trebleClef());
        attributes->setClef(2, dom::Clef::bassClef());

        measure->addNode(std::move(attributes));
        part->addMeasure(std::move(measure));

        return score;
    }

    std::unique_ptr<dom::Chord> ScoreFactory::buildChord(dom::time_t time, int octave, dom::Pitch::Step step, int alter) {
        auto chord = std::unique_ptr<dom::Chord>(new dom::Chord{});

        auto note = std::unique_ptr<dom::Note>(new dom::Note{});
        note->setParent(chord.get());
        note->setStart(time);
        
        auto pitch = std::unique_ptr<dom::Pitch>(new dom::Pitch{});
        pitch->setOctave(octave);
        pitch->setStep(step);
        pitch->setAlter(alter);
        pitch->setParent(note.get());

        note->setPitch(std::move(pitch));
        chord->addNote(std::move(note));
        return chord;
    }
}
}
