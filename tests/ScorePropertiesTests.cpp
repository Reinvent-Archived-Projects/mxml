//  Created by Alejandro Isaza on 2015-01-06.
//  Copyright (c) 2015 Venture Media Labs. All rights reserved.

#include <mxml/ScoreBuilder.h>
#include <mxml/dom/Chord.h>
#include <mxml/ScoreProperties.h>
#include <boost/test/unit_test.hpp>

using namespace mxml;

BOOST_AUTO_TEST_CASE(defaultDivisions) {
    ScoreBuilder builder;
    builder.addPart();

    auto score = builder.build();
    ScoreProperties properties(*score);

    BOOST_CHECK_EQUAL(properties.divisions(0), 1);
    BOOST_CHECK_EQUAL(properties.divisions(20), 1);
}

BOOST_AUTO_TEST_CASE(onlyDivisions) {
    static const int kInitialDivisions = 8;

    ScoreBuilder builder;
    auto part = builder.addPart();
    auto measure = builder.addMeasure(part);
    auto attributes = builder.addAttributes(measure);
    attributes->setDivisions(dom::presentOptional(kInitialDivisions));

    auto score = builder.build();
    ScoreProperties properties(*score);

    BOOST_CHECK_EQUAL(properties.divisions(0), 8);
    BOOST_CHECK_EQUAL(properties.divisions(20), 8);
}

BOOST_AUTO_TEST_CASE(varyingDivisions) {
    static const int kInitialDivisions = 8;
    static const int kModifiedDivisions = 16;

    ScoreBuilder builder;
    auto part = builder.addPart();

    // First measure defines the divisions
    auto measure1 = builder.addMeasure(part);
    auto attributes1 = builder.addAttributes(measure1);
    attributes1->setDivisions(dom::presentOptional(kInitialDivisions));

    // Second measure doesn't define any attributes
    builder.addMeasure(part);

    // Third measure modifes the divisions value
    auto measure3 = builder.addMeasure(part);
    auto attributes3 = builder.addAttributes(measure3);
    attributes3->setDivisions(dom::presentOptional(kModifiedDivisions));

    auto score = builder.build();
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

    ScoreBuilder builder;
    auto part = builder.addPart();

    // Create a measure defines a key signature and then a couple of alters
    auto measure1 = builder.addMeasure(part);
    auto attributes1 = builder.addAttributes(measure1);

    auto clef = dom::Clef::trebleClef();
    clef->setParent(attributes1);
    attributes1->setClef(1, std::move(clef));

    auto key = std::unique_ptr<dom::Key>(new dom::Key{});
    key->setParent(attributes1);
    key->setFifths(2);
    attributes1->setKey(1, std::move(key));

    auto chord1 = builder.addChord(measure1);
    auto note1 = builder.addNote(chord1);
    note1->setStart(1);
    note1->setStaff(kStaff);
    
    auto pitch1 = builder.setPitch(note1);
    pitch1->setOctave(kOctave);
    pitch1->setStep(dom::Pitch::STEP_C);
    pitch1->setAlter(0);

    auto chord2 = builder.addChord(measure1);
    auto note2 = builder.addNote(chord2);
    note2->setStart(2);
    note2->setStaff(kStaff);

    auto pitch2 = builder.setPitch(note2);
    pitch2->setOctave(kOctave);
    pitch2->setStep(dom::Pitch::STEP_D);
    pitch2->setAlter(1);

    auto score = builder.build();
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
