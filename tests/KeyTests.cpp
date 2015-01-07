//  Created by Alejandro Isaza on 2015-01-06.
//  Copyright (c) 2015 Venture Media Labs. All rights reserved.

#include <mxml/dom/Key.h>
#include <boost/test/unit_test.hpp>

using namespace mxml::dom;

BOOST_AUTO_TEST_CASE(alterForCMajor) {
    Key key;
    key.setFifths(0);
    BOOST_CHECK_EQUAL(key.alter(Pitch::STEP_C), 0);
    BOOST_CHECK_EQUAL(key.alter(Pitch::STEP_D), 0);
    BOOST_CHECK_EQUAL(key.alter(Pitch::STEP_E), 0);
    BOOST_CHECK_EQUAL(key.alter(Pitch::STEP_F), 0);
    BOOST_CHECK_EQUAL(key.alter(Pitch::STEP_G), 0);
    BOOST_CHECK_EQUAL(key.alter(Pitch::STEP_A), 0);
    BOOST_CHECK_EQUAL(key.alter(Pitch::STEP_B), 0);
}

BOOST_AUTO_TEST_CASE(alterForGMajor) {
    Key key;
    key.setFifths(1);
    BOOST_CHECK_EQUAL(key.alter(Pitch::STEP_C), 0);
    BOOST_CHECK_EQUAL(key.alter(Pitch::STEP_D), 0);
    BOOST_CHECK_EQUAL(key.alter(Pitch::STEP_E), 0);
    BOOST_CHECK_EQUAL(key.alter(Pitch::STEP_F), 1);
    BOOST_CHECK_EQUAL(key.alter(Pitch::STEP_G), 0);
    BOOST_CHECK_EQUAL(key.alter(Pitch::STEP_A), 0);
    BOOST_CHECK_EQUAL(key.alter(Pitch::STEP_B), 0);
}

BOOST_AUTO_TEST_CASE(alterForBMajor) {
    Key key;
    key.setFifths(5);
    BOOST_CHECK_EQUAL(key.alter(Pitch::STEP_C), 1);
    BOOST_CHECK_EQUAL(key.alter(Pitch::STEP_D), 1);
    BOOST_CHECK_EQUAL(key.alter(Pitch::STEP_E), 0);
    BOOST_CHECK_EQUAL(key.alter(Pitch::STEP_F), 1);
    BOOST_CHECK_EQUAL(key.alter(Pitch::STEP_G), 1);
    BOOST_CHECK_EQUAL(key.alter(Pitch::STEP_A), 1);
    BOOST_CHECK_EQUAL(key.alter(Pitch::STEP_B), 0);
}

BOOST_AUTO_TEST_CASE(alterForFMajor) {
    Key key;
    key.setFifths(-1);
    BOOST_CHECK_EQUAL(key.alter(Pitch::STEP_C), 0);
    BOOST_CHECK_EQUAL(key.alter(Pitch::STEP_D), 0);
    BOOST_CHECK_EQUAL(key.alter(Pitch::STEP_E), 0);
    BOOST_CHECK_EQUAL(key.alter(Pitch::STEP_F), 0);
    BOOST_CHECK_EQUAL(key.alter(Pitch::STEP_G), 0);
    BOOST_CHECK_EQUAL(key.alter(Pitch::STEP_A), 0);
    BOOST_CHECK_EQUAL(key.alter(Pitch::STEP_B), -1);
}

BOOST_AUTO_TEST_CASE(alterForGFlatMajor) {
    Key key;
    key.setFifths(-6);
    BOOST_CHECK_EQUAL(key.alter(Pitch::STEP_C), -1);
    BOOST_CHECK_EQUAL(key.alter(Pitch::STEP_D), -1);
    BOOST_CHECK_EQUAL(key.alter(Pitch::STEP_E), -1);
    BOOST_CHECK_EQUAL(key.alter(Pitch::STEP_F), 0);
    BOOST_CHECK_EQUAL(key.alter(Pitch::STEP_G), -1);
    BOOST_CHECK_EQUAL(key.alter(Pitch::STEP_A), -1);
    BOOST_CHECK_EQUAL(key.alter(Pitch::STEP_B), -1);
}
