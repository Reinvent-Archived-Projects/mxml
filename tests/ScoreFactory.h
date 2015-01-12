//  Created by Alejandro Isaza on 2015-01-12.
//  Copyright (c) 2015 Venture Media Labs. All rights reserved.

#pragma once
#include <mxml/dom/Chord.h>
#include <mxml/dom/Score.h>
#include <memory>

namespace mxml {
namespace tests {
    class ScoreFactory {
    public:
        /**
         Create a score with one part and no measures.
         */
        static std::unique_ptr<dom::Score> buildScore();

        /**
         Create a score with one part, one measure and one set of attributes defining 1 staff and 8 divisions.
         */
        static std::unique_ptr<dom::Score> buildScoreWithAttributes();

        /**
         Create a score with one part and one measure with two staves, each with a clef.
         */
        static std::unique_ptr<dom::Score> buildScorePiano();

        /**
         Create a chord with a single note with the given step, octave and alter.
         */
        static std::unique_ptr<dom::Chord> buildChord(dom::time_t time, int octave, dom::Pitch::Step step, int alter);
    };
}
}