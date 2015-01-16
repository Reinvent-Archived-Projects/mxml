//  Created by Alejandro Isaza on 2014-05-20.
//  Copyright (c) 2014 Venture Media Labs. All rights reserved.

#include <lxml/lxml.h>
#include <mxml/parsing/ScoreHandler.h>
#include <mxml/EventFactory.h>
#include <mxml/ScoreBuilder.h>

#include <fstream>
#include <boost/test/unit_test.hpp>
#include <unistd.h>

using namespace mxml;

BOOST_AUTO_TEST_CASE(moonlight) {
    ScoreHandler handler;
    std::ifstream is("moonlight.xml");
    lxml::parse(is, "moonlight.xml", handler);

    const dom::Score& score = *handler.result();
    ScoreProperties scoreProperties(score);

    EventFactory factory(score, scoreProperties);
    auto events = factory.build();

    // Total number of events including repeats
    BOOST_CHECK_EQUAL(events->events().size(), 3737);

    BOOST_CHECK_EQUAL(scoreProperties.tempo(0, 0), 160);
    BOOST_CHECK_CLOSE(scoreProperties.tempo(13, 240), 80, 0.01);
    BOOST_CHECK_EQUAL(scoreProperties.tempo(14, 0), 160);
}

BOOST_AUTO_TEST_CASE(beats4_4) {
    ScoreBuilder builder;
    auto part = builder.addPart();
    auto measure = builder.addMeasure(part);

    auto attributes = builder.addAttributes(measure);
    attributes->setDivisions(dom::presentOptional(8));

    auto time = builder.setTime(attributes);
    time->setBeats(4);
    time->setBeatType(4);

    auto score = builder.build();
    ScoreProperties scoreProperties(*score);

    EventFactory factory(*score, scoreProperties);
    auto events = factory.build();

    BOOST_CHECK_EQUAL(events->events().size(), 4);

    auto it = events->begin();
    BOOST_CHECK_EQUAL(it->measureIndex(), 0);
    BOOST_CHECK_EQUAL(it->measureTime(), 0);
    BOOST_CHECK(it->isBeatMark());

    ++it;
    BOOST_CHECK_EQUAL(it->measureIndex(), 0);
    BOOST_CHECK_EQUAL(it->measureTime(), 8);
    BOOST_CHECK(it->isBeatMark());

    ++it;
    BOOST_CHECK_EQUAL(it->measureIndex(), 0);
    BOOST_CHECK_EQUAL(it->measureTime(), 16);
    BOOST_CHECK(it->isBeatMark());

    ++it;
    BOOST_CHECK_EQUAL(it->measureIndex(), 0);
    BOOST_CHECK_EQUAL(it->measureTime(), 24);
    BOOST_CHECK(it->isBeatMark());
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

    auto score = builder.build();
    ScoreProperties scoreProperties(*score);

    EventFactory factory(*score, scoreProperties);
    auto events = factory.build();

    BOOST_CHECK_EQUAL(events->events().size(), 2);

    auto it = events->begin();
    BOOST_CHECK_EQUAL(it->measureIndex(), 0);
    BOOST_CHECK_EQUAL(it->measureTime(), 0);
    BOOST_CHECK(it->isBeatMark());

    ++it;
    BOOST_CHECK_EQUAL(it->measureIndex(), 0);
    BOOST_CHECK_EQUAL(it->measureTime(), 8);
    BOOST_CHECK(it->isBeatMark());
}
