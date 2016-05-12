// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <mxml/ScoreBuilder.h>
#include <mxml/ScrollMetrics.h>

#include <boost/test/unit_test.hpp>

using namespace mxml;
using namespace dom;

BOOST_AUTO_TEST_CASE(noteStaffY) {
    ScoreBuilder builder;
    auto part = builder.addPart();
    auto measure = builder.addMeasure(part);
    auto attributes = builder.addAttributes(measure);
    attributes->setStaves(dom::presentOptional(2));
    builder.setTrebleClef(attributes, 1);
    builder.setBassClef(attributes, 2);

    auto score = builder.build();
    ScoreProperties scoreProperties(*score, ScoreProperties::LayoutType::Scroll);
    ScrollMetrics metrics(*score, scoreProperties, 0);

    Note note;
    note.setMeasure(measure);

    note.pitch = std::unique_ptr<Pitch>(new Pitch(Pitch::Step::G, 0, 4));
    BOOST_CHECK_EQUAL(metrics.staffY(note), 30);
    
    note.pitch = std::unique_ptr<Pitch>(new Pitch(Pitch::Step::G, 0, 5));
    BOOST_CHECK_EQUAL(metrics.staffY(note), -5);
    
    note.pitch = std::unique_ptr<Pitch>(new Pitch(Pitch::Step::G, 0, 3));
    BOOST_CHECK_EQUAL(metrics.staffY(note), 65);
}

BOOST_AUTO_TEST_CASE(noteStaff2Y) {
    ScoreBuilder builder;
    auto part = builder.addPart();
    auto measure = builder.addMeasure(part);
    auto attributes = builder.addAttributes(measure);
    attributes->setStaves(dom::presentOptional(2));
    builder.setTrebleClef(attributes, 1);
    builder.setBassClef(attributes, 2);

    auto score = builder.build();
    ScoreProperties scoreProperties(*score, ScoreProperties::LayoutType::Scroll);
    ScrollMetrics metrics(*score, scoreProperties, 0);
    
    Note note;
    note.setStaff(2);
    note.setMeasure(measure);
    
    note.pitch = std::unique_ptr<Pitch>(new Pitch(Pitch::Step::F, 0, 3));
    BOOST_CHECK_EQUAL(metrics.staffY(note), 10);
    
    note.pitch = std::unique_ptr<Pitch>(new Pitch(Pitch::Step::C, 0, 3));
    BOOST_CHECK_EQUAL(metrics.staffY(note), 25);
}

BOOST_AUTO_TEST_CASE(staffOrigin) {
    ScoreBuilder builder;
    auto part = builder.addPart();
    auto measure = builder.addMeasure(part);
    auto attributes = builder.addAttributes(measure);
    attributes->setStaves(dom::presentOptional(2));
    builder.setTrebleClef(attributes, 1);
    builder.setBassClef(attributes, 2);

    auto score = builder.build();
    ScoreProperties scoreProperties(*score, ScoreProperties::LayoutType::Scroll);
    ScrollMetrics metrics(*score, scoreProperties, 0);
    
    BOOST_CHECK_EQUAL(metrics.staffOrigin(1), 0);
    BOOST_CHECK_EQUAL(metrics.staffOrigin(2), Metrics::staffHeight() + 65);
}

BOOST_AUTO_TEST_CASE(noteY) {
    ScoreBuilder builder;
    auto part = builder.addPart();
    auto measure = builder.addMeasure(part);
    auto attributes = builder.addAttributes(measure);
    attributes->setStaves(dom::presentOptional(2));
    builder.setTrebleClef(attributes, 1);
    builder.setBassClef(attributes, 2);

    auto score = builder.build();
    ScoreProperties scoreProperties(*score, ScoreProperties::LayoutType::Scroll);
    ScrollMetrics metrics(*score, scoreProperties, 0);
    
    Note note;
    note.setMeasure(measure);
    note.setStaff(1);
    note.pitch = std::unique_ptr<Pitch>(new Pitch(Pitch::Step::G, 0, 4));
    BOOST_CHECK_EQUAL(metrics.noteY(note), 30);
    
    note.setStaff(2);
    note.pitch = std::unique_ptr<Pitch>(new Pitch(Pitch::Step::F, 0, 3));
    BOOST_CHECK_EQUAL(metrics.noteY(note), Metrics::staffHeight() + 65 + 10);
}
