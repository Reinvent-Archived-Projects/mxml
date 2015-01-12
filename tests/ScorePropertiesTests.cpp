//  Created by Alejandro Isaza on 2015-01-06.
//  Copyright (c) 2015 Venture Media Labs. All rights reserved.

#include "ScoreFactory.h"

#include <mxml/dom/Chord.h>
#include <mxml/ScoreProperties.h>
#include <boost/test/unit_test.hpp>

using namespace mxml;

BOOST_AUTO_TEST_CASE(defaultDivisions) {
    auto score = tests::ScoreFactory::buildScore();
    ScoreProperties properties(*score);

    BOOST_CHECK_EQUAL(properties.divisions(0), 1);
    BOOST_CHECK_EQUAL(properties.divisions(20), 1);
}

BOOST_AUTO_TEST_CASE(onlyDivisions) {
    auto score = tests::ScoreFactory::buildScoreWithAttributes();
    ScoreProperties properties(*score);

    BOOST_CHECK_EQUAL(properties.divisions(0), 8);
    BOOST_CHECK_EQUAL(properties.divisions(20), 8);
}

BOOST_AUTO_TEST_CASE(varyingDivisions) {
    static const int kInitialDivisions = 8;
    static const int kModifiedDivisions = 16;

    auto score = tests::ScoreFactory::buildScoreWithAttributes();
    auto& part = score->parts().at(0);


    // Second measure doesn't define any attributes
    auto measure2 = std::unique_ptr<dom::Measure>(new dom::Measure{});
    measure2->setIndex(1);
    part->addMeasure(std::move(measure2));


    // Third measure modifes the divisions value
    auto measure3 = std::unique_ptr<dom::Measure>(new dom::Measure{});
    measure3->setIndex(2);

    auto attributes3 = std::unique_ptr<dom::Attributes>(new dom::Attributes{});
    attributes3->setParent(measure3.get());
    attributes3->setStart(0);
    attributes3->setDivisions(dom::presentOptional(kModifiedDivisions));
    measure3->addNode(std::move(attributes3));

    part->addMeasure(std::move(measure3));

    ScoreProperties properties(*score);
    BOOST_CHECK_EQUAL(properties.divisions(0), kInitialDivisions);
    BOOST_CHECK_EQUAL(properties.divisions(1), kInitialDivisions);
    BOOST_CHECK_EQUAL(properties.divisions(2), kModifiedDivisions);
    BOOST_CHECK_EQUAL(properties.divisions(3), kModifiedDivisions);
}

BOOST_AUTO_TEST_CASE(alters) {
    static const std::size_t kPartIndex = 0;
    static const std::size_t kMeasureIndex = 0;
    static const int kStaff = 1;
    static const int kOctave = 4;

    auto score = tests::ScoreFactory::buildScoreWithAttributes();
    auto& part = score->parts().at(0);
    auto& measure1 = part->measures().at(0);

    // Create a measure defines a key signature and then a couple of alters
    auto attributes1 = std::unique_ptr<dom::Attributes>(new dom::Attributes{});
    attributes1->setParent(measure1.get());
    attributes1->setStart(0);

    auto key = std::unique_ptr<dom::Key>(new dom::Key{});
    key->setFifths(2);
    attributes1->setKey(1, std::move(key));

    measure1->addNode(std::move(attributes1));

    auto chord1 = tests::ScoreFactory::buildChord(1, kOctave, dom::Pitch::STEP_C, 0);
    measure1->addNode(std::move(chord1));

    auto chord2 = tests::ScoreFactory::buildChord(2, kOctave, dom::Pitch::STEP_D, 1);
    measure1->addNode(std::move(chord2));

    ScoreProperties properties(*score);
    
    // Note C is initially 1 from the key signature and gets modified at time 1 back to 0
    BOOST_CHECK_EQUAL(properties.alter(kPartIndex, kMeasureIndex, 0, kStaff, kOctave, dom::Pitch::STEP_C), 1);
    BOOST_CHECK_EQUAL(properties.alter(kPartIndex, kMeasureIndex, 1, kStaff, kOctave, dom::Pitch::STEP_C), 0);
    BOOST_CHECK_EQUAL(properties.alter(kPartIndex, kMeasureIndex, 2, kStaff, kOctave, dom::Pitch::STEP_C), 0);

    // Note D is initially 0 and gets modified at time 2
    BOOST_CHECK_EQUAL(properties.alter(kPartIndex, kMeasureIndex, 0, kStaff, kOctave, dom::Pitch::STEP_D), 0);
    BOOST_CHECK_EQUAL(properties.alter(kPartIndex, kMeasureIndex, 1, kStaff, kOctave, dom::Pitch::STEP_D), 0);
    BOOST_CHECK_EQUAL(properties.alter(kPartIndex, kMeasureIndex, 2, kStaff, kOctave, dom::Pitch::STEP_D), 1);

    // Key signature modifications should not be preserved across measures
    BOOST_CHECK_EQUAL(properties.alter(kPartIndex, kMeasureIndex + 1, 0, kStaff, kOctave, dom::Pitch::STEP_C), 1);
    BOOST_CHECK_EQUAL(properties.alter(kPartIndex, kMeasureIndex + 1, 0, kStaff, kOctave, dom::Pitch::STEP_D), 0);
}
