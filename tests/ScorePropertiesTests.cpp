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
    static const int kStaff = 1;
    static const int kOctave = 4;

    ScoreBuilder builder;
    auto part = builder.addPart();

    // Create a measure defines a key signature and then a couple of alters
    auto measure1 = builder.addMeasure(part);
    auto measure2 = builder.addMeasure(part);
    auto attributes1 = builder.addAttributes(measure1);

    auto clef = dom::Clef::trebleClef();
    clef->setParent(attributes1);
    attributes1->setClef(1, std::move(clef));

    // This key signature modifies F and C by +1
    auto key = std::unique_ptr<dom::Key>(new dom::Key{});
    key->setParent(attributes1);
    key->setFifths(2);
    attributes1->setKey(1, std::move(key));
    
    // C Steps - measure 1
    auto chord1 = builder.addChord(measure1);
    auto note1 = builder.addNote(chord1, dom::Note::TYPE_EIGHTH, 1);
    note1->setStaff(kStaff);
    builder.setPitch(note1, dom::Pitch::STEP_C, kOctave, {-1, true});
    
    auto chord2 = builder.addChord(measure1);
    auto note2 = builder.addNote(chord2, dom::Note::TYPE_EIGHTH, 2);
    note2->setStaff(kStaff);
    builder.setPitch(note2, dom::Pitch::STEP_C, kOctave, {1, true});
    
    // D Steps - measure 1
    auto chord3 = builder.addChord(measure1);
    auto note3 = builder.addNote(chord3, dom::Note::TYPE_EIGHTH, 1);
    note3->setStaff(kStaff);
    builder.setPitch(note3, dom::Pitch::STEP_D, kOctave, {1, true});
    
    auto chord4 = builder.addChord(measure1);
    auto note4 = builder.addNote(chord4, dom::Note::TYPE_EIGHTH, 2);
    note4->setStaff(kStaff);
    builder.setPitch(note4, dom::Pitch::STEP_D, kOctave, {-1, true});
    
    // C Steps - measure 2
    auto chord5 = builder.addChord(measure2);
    auto note5 = builder.addNote(chord5, dom::Note::TYPE_EIGHTH, 1);
    note5->setStaff(kStaff);
    builder.setPitch(note5, dom::Pitch::STEP_C, kOctave);
    
    auto score = builder.build();
    ScoreProperties properties(*score);
    
    // Original key signature
    BOOST_CHECK_EQUAL(properties.previousAlter(*note1), 1);
    
    // Note C - measure 1 - time 1
    BOOST_CHECK_EQUAL(properties.previousAlter(*note1), 1);
    BOOST_CHECK_EQUAL(properties.alter(*note1), -1);
    
    // Note C - measure 1 - time 2
    BOOST_CHECK_EQUAL(properties.previousAlter(*note2), -1);
    BOOST_CHECK_EQUAL(properties.alter(*note2), 1);

    // Note D - measure 1 - time 1
    BOOST_CHECK_EQUAL(properties.previousAlter(*note3), 0);
    BOOST_CHECK_EQUAL(properties.alter(*note3), 1);
    
    // Note D - measure 1 - time 2
    BOOST_CHECK_EQUAL(properties.previousAlter(*note4), 1);
    BOOST_CHECK_EQUAL(properties.alter(*note4), -1);
    
    // Key signature modifications should not be preserved across measures
    // Note C - measure 2 - time 1
    BOOST_CHECK_EQUAL(properties.alter(*note5), 1);
}
