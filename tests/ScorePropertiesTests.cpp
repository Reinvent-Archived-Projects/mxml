// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <mxml/ScoreBuilder.h>
#include <mxml/dom/Chord.h>
#include <mxml/ScoreProperties.h>
#include <mxml/StreamOperators.h>
#include <boost/test/unit_test.hpp>

using namespace mxml;

BOOST_AUTO_TEST_CASE(defaultDivisions) {
    ScoreBuilder builder;
    builder.addPart();

    auto score = builder.build();
    ScoreProperties properties(*score, ScoreProperties::LayoutType::Scroll);

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
    ScoreProperties properties(*score, ScoreProperties::LayoutType::Scroll);

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
    ScoreProperties properties(*score, ScoreProperties::LayoutType::Scroll);

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
    auto chord_c1 = builder.addChord(measure1);
    auto note_c1 = builder.addNote(chord_c1, dom::Note::Type::Eighth, 1);
    note_c1->setStaff(kStaff);
    builder.setPitch(note_c1, dom::Pitch::Step::C, kOctave, -1);
    
    auto chord_c2 = builder.addChord(measure1);
    auto note_c2 = builder.addNote(chord_c2, dom::Note::Type::Eighth, 2);
    note_c2->setStaff(kStaff);
    builder.setPitch(note_c2, dom::Pitch::Step::C, kOctave, 0);
    
    // D Steps - measure 1
    auto chord_d1 = builder.addChord(measure1);
    auto note_d1 = builder.addNote(chord_d1, dom::Note::Type::Eighth, 1);
    note_d1->setStaff(kStaff);
    builder.setPitch(note_d1, dom::Pitch::Step::D, kOctave, 1);
    
    auto chord_d2 = builder.addChord(measure1);
    auto note_d2 = builder.addNote(chord_d2, dom::Note::Type::Eighth, 2);
    note_d2->setStaff(kStaff);
    builder.setPitch(note_d2, dom::Pitch::Step::D, kOctave, 0);
    
    // C Steps - measure 2
    auto chord_c3 = builder.addChord(measure2);
    auto note_c3 = builder.addNote(chord_c3, dom::Note::Type::Eighth, 1);
    note_c3->setStaff(kStaff);
    builder.setPitch(note_c3, dom::Pitch::Step::C, kOctave);
    
    auto score = builder.build();
    ScoreProperties properties(*score, ScoreProperties::LayoutType::Scroll);
    
    // Original key signature
    BOOST_CHECK_EQUAL(properties.alter(*note_c1), 1);
    
    // Note C - measure 1 - time 2
    BOOST_CHECK_EQUAL(properties.alter(*note_c2), -1);

    // Note D - measure 1 - time 1
    BOOST_CHECK_EQUAL(properties.alter(*note_d1), 0);
    
    // Note D - measure 1 - time 2
    BOOST_CHECK_EQUAL(properties.alter(*note_d2), 1);
    
    // Key signature modifications should not be preserved across measures
    // Note C - measure 2 - time 1
    BOOST_CHECK_EQUAL(properties.alter(*note_c3), 1);
}

BOOST_AUTO_TEST_CASE(altersSecondStaff) {
    ScoreBuilder builder;
    auto part = builder.addPart();

    // Create a measure defines a key signature and then a couple of alters
    auto measure1 = builder.addMeasure(part);

    auto attributes1 = builder.addAttributes(measure1);
    attributes1->setStaves({2, true});

    auto clef1 = dom::Clef::trebleClef();
    clef1->setParent(attributes1);
    attributes1->setClef(1, std::move(clef1));

    auto clef2 = dom::Clef::bassClef();
    clef2->setParent(attributes1);
    attributes1->setClef(2, std::move(clef2));

    // This key signature modifies F and C by +1
    auto key = std::unique_ptr<dom::Key>(new dom::Key{});
    key->setParent(attributes1);
    key->setFifths(2);
    attributes1->setKey(1, std::move(key));

    // C Steps - staff 1
    auto chord_c1 = builder.addChord(measure1);
    auto note_c1 = builder.addNote(chord_c1, dom::Note::Type::Eighth, 1);
    note_c1->setStaff(1);
    builder.setPitch(note_c1, dom::Pitch::Step::C, 4);

    // C Steps - staff 2
    auto chord_c2 = builder.addChord(measure1);
    auto note_c2 = builder.addNote(chord_c2, dom::Note::Type::Eighth, 1);
    note_c2->setStaff(2);
    builder.setPitch(note_c2, dom::Pitch::Step::C, 5);

    auto score = builder.build();
    ScoreProperties properties(*score, ScoreProperties::LayoutType::Scroll);

    // Even though the key signature is only set for the first staff, it should apply to the second staff as well
    BOOST_CHECK_EQUAL(properties.alter(*note_c1), 1);
    BOOST_CHECK_EQUAL(properties.alter(*note_c2), 1);
}

BOOST_AUTO_TEST_CASE(keySignatures) {
    static const int kFifths = 2;

    ScoreBuilder builder;
    auto part = builder.addPart();
    auto measure = builder.addMeasure(part);
    auto attributes = builder.addAttributes(measure);
    attributes->setStaves({2, true});

    auto key = std::unique_ptr<dom::Key>(new dom::Key{});
    key->setParent(attributes);
    key->setFifths(kFifths);
    attributes->setKey(1, std::move(key));

    auto score = builder.build();
    ScoreProperties proeprties(*score, ScoreProperties::LayoutType::Scroll);

    // Even though the key was only specified for the first staff it should apply to all staves
    BOOST_CHECK_EQUAL(proeprties.key(0, 0, 1, 0)->fifths(), kFifths);
    BOOST_CHECK_EQUAL(proeprties.key(0, 0, 2, 0)->fifths(), kFifths);
}

BOOST_AUTO_TEST_CASE(clefSignatures) {
    ScoreBuilder builder;
    auto part = builder.addPart();
    auto measure1 = builder.addMeasure(part);
    auto attributes1 = builder.addAttributes(measure1);
    attributes1->setStaves({2, true});

    auto trebleClef = dom::Clef::trebleClef();
    trebleClef->setParent(attributes1);
    attributes1->setClef(1, std::move(trebleClef));

    auto measure2 = builder.addMeasure(part);
    auto attributes2 = builder.addAttributes(measure2);

    auto bassClef = dom::Clef::bassClef();
    bassClef->setParent(attributes2);
    attributes2->setClef(2, std::move(bassClef));

    auto score = builder.build();
    ScoreProperties proeprties(*score, ScoreProperties::LayoutType::Scroll);

    // Measure 1
    // Even though the clef was only specified for the first staff it should apply to all staves
    BOOST_CHECK(proeprties.clef(0, 0, 1, 0)->sign() == dom::Clef::Sign::G);
    BOOST_CHECK(proeprties.clef(0, 0, 2, 0)->sign() == dom::Clef::Sign::G);

    // Measure 2
    BOOST_CHECK(proeprties.clef(0, 1, 1, 0)->sign() == dom::Clef::Sign::G);
    BOOST_CHECK(proeprties.clef(0, 1, 2, 0)->sign() == dom::Clef::Sign::F);
}

BOOST_AUTO_TEST_CASE(clefSignaturesOnlyFirstStaff) {
    ScoreBuilder builder;
    auto part = builder.addPart();
    auto measure1 = builder.addMeasure(part);
    auto attributes1 = builder.addAttributes(measure1);
    attributes1->setStaves({2, true});

    auto trebleClef = dom::Clef::trebleClef();
    trebleClef->setParent(attributes1);
    attributes1->setClef(1, std::move(trebleClef));

    auto measure2 = builder.addMeasure(part);
    auto attributes2 = builder.addAttributes(measure2);

    auto bassClef = dom::Clef::bassClef();
    bassClef->setParent(attributes2);
    attributes2->setClef(1, std::move(bassClef));

    auto score = builder.build();
    ScoreProperties proeprties(*score, ScoreProperties::LayoutType::Scroll);

    // Measure 1
    // Even though the clef was only specified for the first staff it should apply to all staves
    BOOST_CHECK(proeprties.clef(0, 0, 1, 0)->sign() == dom::Clef::Sign::G);
    BOOST_CHECK(proeprties.clef(0, 0, 2, 0)->sign() == dom::Clef::Sign::G);

    // Measure 2
    BOOST_CHECK(proeprties.clef(0, 1, 1, 0)->sign() == dom::Clef::Sign::F);
    BOOST_CHECK(proeprties.clef(0, 1, 2, 0)->sign() == dom::Clef::Sign::F);
}
