// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <lxml/lxml.h>
#include <mxml/parsing/ScoreHandler.h>
#include <mxml/EventFactory.h>
#include <mxml/ScoreBuilder.h>

#include <fstream>
#include <boost/test/unit_test.hpp>
#include <unistd.h>

using namespace mxml;
using namespace mxml::parsing;

static const char* kMoonlightFileName               = "moonlight.xml";
static const char* kEventsFileName                  = "events.xml";
static const char* kEventsRepeatFileName            = "events_repeat.xml";
static const char* kEventsRepeatLastMeasureFileName = "events_repeat_last_measure.xml";
static const char* kEventsDSAlCodaFileName          = "events_ds_al_coda.xml";
static const char* kEventsComplex1FileName          = "events_complex_1.xml";
static const char* kEventsComplex2FileName          = "events_complex_2.xml";

BOOST_AUTO_TEST_CASE(moonlight) {
    ScoreHandler handler;
    std::ifstream is(kMoonlightFileName);
    lxml::parse(is, kMoonlightFileName, handler);

    const dom::Score& score = *handler.result();
    ScoreProperties scoreProperties(score, ScoreProperties::LayoutType::Scroll);

    EventFactory factory(score, scoreProperties);
    auto events = factory.build();

    // Total number of events including repeats
    BOOST_CHECK_EQUAL(events->events().size(), 3737);

    BOOST_CHECK_EQUAL(scoreProperties.tempo(0, 0), 160);
    BOOST_CHECK_CLOSE(scoreProperties.tempo(13, 240), 80, 0.01);
    BOOST_CHECK_EQUAL(scoreProperties.tempo(14, 0), 160);
}

BOOST_AUTO_TEST_CASE(beats_short_measure) {
    ScoreBuilder builder;
    auto part = builder.addPart();

    auto measure1 = builder.addMeasure(part);
    auto attributes1 = builder.addAttributes(measure1);
    attributes1->setDivisions(dom::presentOptional(1));
    auto time = builder.setTime(attributes1);
    time->setBeats(4);
    time->setBeatType(4);

    auto measure2 = builder.addMeasure(part);

    builder.addNote(measure1, dom::Note::Type::Quarter, 0);
    builder.addNote(measure2, dom::Note::Type::Quarter, 0);
    builder.addNote(measure2, dom::Note::Type::Quarter, 1);
    builder.addNote(measure2, dom::Note::Type::Quarter, 2);
    builder.addNote(measure2, dom::Note::Type::Quarter, 4);

    auto score = builder.build();
    ScoreProperties scoreProperties(*score, ScoreProperties::LayoutType::Scroll);

    EventFactory factory(*score, scoreProperties);
    auto events = factory.build();

    BOOST_CHECK_EQUAL(events->events().size(), 6);

    auto& beatEvent = events->events().at(0);
    BOOST_CHECK_EQUAL(beatEvent.measureIndex(), 0);
    BOOST_CHECK_EQUAL(beatEvent.measureTime(), 0);
    BOOST_CHECK(beatEvent.isBeatMark());

    beatEvent = events->events().at(1);
    BOOST_CHECK_EQUAL(beatEvent.measureIndex(), 1);
    BOOST_CHECK_EQUAL(beatEvent.measureTime(), 0);
    BOOST_CHECK(beatEvent.isBeatMark());

    beatEvent = events->events().at(2);
    BOOST_CHECK_EQUAL(beatEvent.measureIndex(), 1);
    BOOST_CHECK_EQUAL(beatEvent.measureTime(), 1);
    BOOST_CHECK(beatEvent.isBeatMark());

    beatEvent = events->events().at(3);
    BOOST_CHECK_EQUAL(beatEvent.measureIndex(), 1);
    BOOST_CHECK_EQUAL(beatEvent.measureTime(), 2);
    BOOST_CHECK(beatEvent.isBeatMark());

    beatEvent = events->events().at(4);
    BOOST_CHECK_EQUAL(beatEvent.measureIndex(), 1);
    BOOST_CHECK_EQUAL(beatEvent.measureTime(), 3);
    BOOST_CHECK(beatEvent.isBeatMark());
}

BOOST_AUTO_TEST_CASE(beats4_4) {
    ScoreBuilder builder;
    auto part = builder.addPart();
    auto measure = builder.addMeasure(part);

    auto attributes = builder.addAttributes(measure);
    attributes->setDivisions(dom::presentOptional(1));

    auto time = builder.setTime(attributes);
    time->setBeats(4);
    time->setBeatType(4);

    builder.addNote(measure, dom::Note::Type::Quarter, 0);
    builder.addNote(measure, dom::Note::Type::Quarter, 1);
    builder.addNote(measure, dom::Note::Type::Quarter, 2);
    builder.addNote(measure, dom::Note::Type::Quarter, 3);

    auto score = builder.build();
    ScoreProperties scoreProperties(*score, ScoreProperties::LayoutType::Scroll);

    EventFactory factory(*score, scoreProperties);
    auto events = factory.build();

    BOOST_CHECK_EQUAL(events->events().size(), 5);

    auto& beatEvent = events->events().at(0);
    BOOST_CHECK_EQUAL(beatEvent.measureIndex(), 0);
    BOOST_CHECK_EQUAL(beatEvent.measureTime(), 0);
    BOOST_CHECK(beatEvent.isBeatMark());

    beatEvent = events->events().at(1);
    BOOST_CHECK_EQUAL(beatEvent.measureIndex(), 0);
    BOOST_CHECK_EQUAL(beatEvent.measureTime(), 1);
    BOOST_CHECK(beatEvent.isBeatMark());

    beatEvent = events->events().at(2);
    BOOST_CHECK_EQUAL(beatEvent.measureIndex(), 0);
    BOOST_CHECK_EQUAL(beatEvent.measureTime(), 2);
    BOOST_CHECK(beatEvent.isBeatMark());

    beatEvent = events->events().at(3);
    BOOST_CHECK_EQUAL(beatEvent.measureIndex(), 0);
    BOOST_CHECK_EQUAL(beatEvent.measureTime(), 3);
    BOOST_CHECK(beatEvent.isBeatMark());
}

BOOST_AUTO_TEST_CASE(beats2_12) {
    ScoreBuilder builder;
    auto part = builder.addPart();
    auto measure = builder.addMeasure(part);

    auto attributes = builder.addAttributes(measure);
    attributes->setDivisions(dom::presentOptional(24));

    auto time = builder.setTime(attributes);
    time->setBeats(2);
    time->setBeatType(12);

    builder.addNote(measure, dom::Note::Type::Quarter, 0, 8);
    builder.addNote(measure, dom::Note::Type::Quarter, 8, 8);

    auto score = builder.build();
    ScoreProperties scoreProperties(*score, ScoreProperties::LayoutType::Scroll);

    EventFactory factory(*score, scoreProperties);
    auto events = factory.build();

    BOOST_CHECK_EQUAL(events->events().size(), 3);

    auto& beatEvent = events->events().at(0);
    BOOST_CHECK_EQUAL(beatEvent.measureIndex(), 0);
    BOOST_CHECK_EQUAL(beatEvent.measureTime(), 0);
    BOOST_CHECK(beatEvent.isBeatMark());

    beatEvent = events->events().at(1);
    BOOST_CHECK_EQUAL(beatEvent.measureIndex(), 0);
    BOOST_CHECK_EQUAL(beatEvent.measureTime(), 8);
    BOOST_CHECK(beatEvent.isBeatMark());
}

BOOST_AUTO_TEST_CASE(event_order) {
    ScoreHandler handler;
    std::ifstream is(kEventsFileName);
    lxml::parse(is, kEventsFileName, handler);
    
    const dom::Score& score = *handler.result();
    ScoreProperties scoreProperties(score, ScoreProperties::LayoutType::Scroll);
    
    EventFactory factory(score, scoreProperties);
    auto events = factory.build();
    
    std::vector<dom::Pitch::Step> event_order{
        dom::Pitch::Step::A,
        dom::Pitch::Step::B,
        dom::Pitch::Step::C,
        dom::Pitch::Step::D
    };
    
    int index = 0;
    for (auto it = events->begin(); it != events->end(); ++it) {
        auto& event = *it;
        if (event.onNotes().size() > 0) {
            auto note = event.onNotes().front();
            BOOST_CHECK(note->pitch->step() == event_order.at(index));
            ++index;
        }
    }
    
    BOOST_CHECK_EQUAL(index, event_order.size());
}

BOOST_AUTO_TEST_CASE(event_order_repeat) {
    ScoreHandler handler;
    std::ifstream is(kEventsRepeatFileName);
    lxml::parse(is, kEventsRepeatFileName, handler);
    
    const dom::Score& score = *handler.result();
    ScoreProperties scoreProperties(score, ScoreProperties::LayoutType::Scroll);
    
    EventFactory factory(score, scoreProperties);
    auto events = factory.build();
    
    std::vector<dom::Pitch::Step> event_order{
        dom::Pitch::Step::A,
        dom::Pitch::Step::B,
        dom::Pitch::Step::C,
        // Perform Repeat
        dom::Pitch::Step::B,
        dom::Pitch::Step::C,
        dom::Pitch::Step::D
    };
    
    int index = 0;
    for (auto it = events->begin(); it != events->end(); ++it) {
        auto& event = *it;
        if (event.onNotes().size() > 0) {
            auto note = event.onNotes().front();
            BOOST_CHECK(note->pitch->step() == event_order.at(index));
            ++index;
        }
    }
    
    BOOST_CHECK_EQUAL(index, event_order.size());
}

BOOST_AUTO_TEST_CASE(event_order_repeat_last_measure) {
    ScoreHandler handler;
    std::ifstream is(kEventsRepeatLastMeasureFileName);
    lxml::parse(is, kEventsRepeatLastMeasureFileName, handler);
    
    const dom::Score& score = *handler.result();
    ScoreProperties scoreProperties(score, ScoreProperties::LayoutType::Scroll);
    
    EventFactory factory(score, scoreProperties);
    auto events = factory.build();
    
    std::vector<dom::Pitch::Step> event_order{
        dom::Pitch::Step::A,
        dom::Pitch::Step::B,
        dom::Pitch::Step::C,
        dom::Pitch::Step::D,
        // Perform Repeat
        dom::Pitch::Step::C,
        dom::Pitch::Step::D
    };
    
    int index = 0;
    for (auto it = events->begin(); it != events->end(); ++it) {
        auto& event = *it;
        if (event.onNotes().size() > 0) {
            auto note = event.onNotes().front();
            BOOST_CHECK(note->pitch->step() == event_order.at(index));
            ++index;
        }
    }
    
    BOOST_CHECK_EQUAL(index, event_order.size());
}

BOOST_AUTO_TEST_CASE(event_order_ds_al_coda) {
    ScoreHandler handler;
    std::ifstream is(kEventsDSAlCodaFileName);
    lxml::parse(is, kEventsDSAlCodaFileName, handler);
    
    const dom::Score& score = *handler.result();
    ScoreProperties scoreProperties(score, ScoreProperties::LayoutType::Scroll);
    
    EventFactory factory(score, scoreProperties);
    auto events = factory.build();
    
    std::vector<dom::Pitch::Step> event_order{
        dom::Pitch::Step::A,
        dom::Pitch::Step::B,
        dom::Pitch::Step::C,
        // Perform D.S. al Coda (goto Segno)
        dom::Pitch::Step::A,
        dom::Pitch::Step::B,
        // Perform To Coda (goto Coda)
        dom::Pitch::Step::D
    };
    
    int index = 0;
    for (auto it = events->begin(); it != events->end(); ++it) {
        auto& event = *it;
        if (event.onNotes().size() > 0) {
            auto note = event.onNotes().front();
            BOOST_CHECK(note->pitch->step() == event_order.at(index));
            ++index;
        }
    }
    
    BOOST_CHECK_EQUAL(index, event_order.size());
}

BOOST_AUTO_TEST_CASE(event_order_complex_1) {
    ScoreHandler handler;
    std::ifstream is(kEventsComplex1FileName);
    lxml::parse(is, kEventsComplex1FileName, handler);
    
    const dom::Score& score = *handler.result();
    ScoreProperties scoreProperties(score, ScoreProperties::LayoutType::Scroll);
    
    EventFactory factory(score, scoreProperties);
    auto events = factory.build();
    
    std::vector<dom::Pitch::Step> event_order{
        dom::Pitch::Step::A,
        dom::Pitch::Step::B,
        dom::Pitch::Step::C,
        // Perform Repeat
        dom::Pitch::Step::B,
        dom::Pitch::Step::C,
        dom::Pitch::Step::D,
        dom::Pitch::Step::E,
        dom::Pitch::Step::F,
        // Perform D.S. al Coda (goto Segno)
        dom::Pitch::Step::A,
        dom::Pitch::Step::B,
        // Perform To Coda (goto Coda)
        dom::Pitch::Step::G,
    };
    
    int index = 0;
    for (auto it = events->begin(); it != events->end(); ++it) {
        auto& event = *it;
        if (event.onNotes().size() > 0) {
            auto note = event.onNotes().front();
            BOOST_CHECK(note->pitch->step() == event_order.at(index));
            ++index;
        }
    }
    
    BOOST_CHECK_EQUAL(index, event_order.size());
}

BOOST_AUTO_TEST_CASE(event_order_complex_2) {
    ScoreHandler handler;
    std::ifstream is(kEventsComplex2FileName);
    lxml::parse(is, kEventsComplex2FileName, handler);
    
    const dom::Score& score = *handler.result();
    ScoreProperties scoreProperties(score, ScoreProperties::LayoutType::Scroll);
    
    EventFactory factory(score, scoreProperties);
    auto events = factory.build();
    
    std::vector<dom::Pitch::Step> event_order{
        dom::Pitch::Step::A,
        dom::Pitch::Step::B,
        dom::Pitch::Step::C,
        // Perform Repeat
        dom::Pitch::Step::B,
        dom::Pitch::Step::C,
        dom::Pitch::Step::D,
        dom::Pitch::Step::E,
        dom::Pitch::Step::F,
        // Perform D.S. al Coda (goto Segno)
        dom::Pitch::Step::A,
        dom::Pitch::Step::B,
        // Perform To Coda (goto Coda)
        dom::Pitch::Step::F,
        dom::Pitch::Step::G,
        // Perform D.S. (goto 2nd Segno)
        dom::Pitch::Step::F,
        dom::Pitch::Step::F,
        dom::Pitch::Step::G,
        dom::Pitch::Step::A,
        dom::Pitch::Step::B,
    };
    
    int index = 0;
    for (auto it = events->begin(); it != events->end(); ++it) {
        auto& event = *it;
        if (event.onNotes().size() > 0) {
            auto note = event.onNotes().front();
            BOOST_CHECK(note->pitch->step() == event_order.at(index));
            ++index;
        }
    }
    
    BOOST_CHECK_EQUAL(index, event_order.size());
}
