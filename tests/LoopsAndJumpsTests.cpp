//  Created by Alejandro Isaza on 2015-01-15.
//  Copyright (c) 2015 Venture Media Labs. All rights reserved.

#include <lxml/lxml.h>
#include <mxml/EventFactory.h>
#include <mxml/JumpFactory.h>
#include <mxml/LoopFactory.h>
#include <mxml/parsing/ScoreHandler.h>

#include <boost/test/unit_test.hpp>
#include <fstream>
#include <unistd.h>


using namespace mxml;

static const char* kLoopsFileName = "loops.xml";
static const char* kRepeatsFileName = "repeats.xml";

BOOST_AUTO_TEST_CASE(loops) {
    ScoreHandler handler;
    std::ifstream is(kLoopsFileName);
    lxml::parse(is, kLoopsFileName, handler);

    const dom::Score& score = *handler.result();
    LoopFactory loopFactory(score);
    auto loops = loopFactory.build();

    BOOST_CHECK_EQUAL(loops.size(), 2);

    // First loop starts at measure 1 and ends at measure 3 (measures 1 and 2 are repeated)
    BOOST_CHECK_EQUAL(loops[0].begin(), 1);
    BOOST_CHECK_EQUAL(loops[0].end(), 3);
    BOOST_CHECK_EQUAL(loops[0].count(), 1);
    BOOST_CHECK(!loops[0].isSkipped(0, 1));
    BOOST_CHECK(!loops[0].isSkipped(0, 2));

    // The second loops has endings. The order should be 3, 4, 3, 5
    BOOST_CHECK_EQUAL(loops[1].begin(), 3);
    BOOST_CHECK_EQUAL(loops[1].end(), 5);
    BOOST_CHECK_EQUAL(loops[1].count(), 1);
    BOOST_CHECK(loops[1].isSkipped(0, 5));
    BOOST_CHECK(loops[1].isSkipped(1, 4));
}

BOOST_AUTO_TEST_CASE(repeats) {
    ScoreHandler handler;
    std::ifstream is(kRepeatsFileName);
    lxml::parse(is, kRepeatsFileName, handler);

    const dom::Score& score = *handler.result();
    JumpFactory jumpFactory(score);
    auto jumps = jumpFactory.build();

    BOOST_CHECK_EQUAL(jumps.size(), 2);

    // To coda jump from measure 2 to 4
    BOOST_CHECK_EQUAL(jumps[0].from, 2);
    BOOST_CHECK_EQUAL(jumps[0].to, 4);

    // Dal segno jump from measure 5 to 0
    BOOST_CHECK_EQUAL(jumps[1].from, 5);
    BOOST_CHECK_EQUAL(jumps[1].to, 0);
}
