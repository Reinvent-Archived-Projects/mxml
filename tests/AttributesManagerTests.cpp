//  Created by Alejandro Isaza on 2015-01-06.
//  Copyright (c) 2015 Venture Media Labs. All rights reserved.

#include <mxml/AttributesManager.h>
#include <boost/test/unit_test.hpp>

using namespace mxml;

BOOST_AUTO_TEST_CASE(defaultDivisions) {
    AttributesManager manager;
    BOOST_CHECK_EQUAL(manager.divisions(), 1);
    BOOST_CHECK_EQUAL(manager.divisions(20), 1);
}

BOOST_AUTO_TEST_CASE(onlyDivisions) {
    AttributesManager manager;

    auto measure = std::unique_ptr<dom::Measure>(new dom::Measure{});
    measure->setIndex(0);

    auto attributes = std::unique_ptr<dom::Attributes>(new dom::Attributes{});
    attributes->setParent(measure.get());
    attributes->setStaves(dom::presentOptional(2));
    attributes->setStart(0);
    attributes->setDivisions(dom::presentOptional(8));
    measure->addNode(std::move(attributes));

    manager.addAllAttributes(*measure);

    BOOST_CHECK_EQUAL(manager.divisions(), 8);
    BOOST_CHECK_EQUAL(manager.divisions(0), 8);
    BOOST_CHECK_EQUAL(manager.divisions(20), 8);
}

BOOST_AUTO_TEST_CASE(varyingDivisions) {
    static const int kInitialDivisions = 8;
    static const int kModifiedDivisions = 16;

    AttributesManager manager;

    // First measure has an initial divisions value
    auto measure1 = std::unique_ptr<dom::Measure>(new dom::Measure{});
    measure1->setIndex(0);

    auto attributes1 = std::unique_ptr<dom::Attributes>(new dom::Attributes{});
    attributes1->setParent(measure1.get());
    attributes1->setStaves(dom::presentOptional(2));
    attributes1->setStart(0);
    attributes1->setDivisions(dom::presentOptional(kInitialDivisions));
    measure1->addNode(std::move(attributes1));
    manager.addAllAttributes(*measure1);

    BOOST_CHECK_EQUAL(manager.divisions(), kInitialDivisions);


    // Second measure doesn't define any attributes
    auto measure2 = std::unique_ptr<dom::Measure>(new dom::Measure{});
    measure2->setIndex(1);
    manager.addAllAttributes(*measure2);

    BOOST_CHECK_EQUAL(manager.divisions(), kInitialDivisions);


    // Third measure modifes the divisions value
    auto measure3 = std::unique_ptr<dom::Measure>(new dom::Measure{});
    measure3->setIndex(2);

    auto attributes3 = std::unique_ptr<dom::Attributes>(new dom::Attributes{});
    attributes3->setParent(measure2.get());
    attributes3->setStart(0);
    attributes3->setDivisions(dom::presentOptional(kModifiedDivisions));
    measure3->addNode(std::move(attributes3));

    manager.addAllAttributes(*measure3);

    BOOST_CHECK_EQUAL(manager.divisions(), kModifiedDivisions);


    BOOST_CHECK_EQUAL(manager.divisions(0), kInitialDivisions);
    BOOST_CHECK_EQUAL(manager.divisions(1), kInitialDivisions);
    BOOST_CHECK_EQUAL(manager.divisions(2), kModifiedDivisions);
    BOOST_CHECK_EQUAL(manager.divisions(3), kModifiedDivisions);
}

BOOST_AUTO_TEST_CASE(alters) {
    static const std::size_t kMeasureIndex = 0;
    static const int kStaff = 1;
    static const int kOctave = 4;

    AttributesManager manager;

    // Create a measure defines a key signature and then a couple of alters
    auto measure1 = std::unique_ptr<dom::Measure>(new dom::Measure{});
    measure1->setIndex(kMeasureIndex);

    auto attributes1 = std::unique_ptr<dom::Attributes>(new dom::Attributes{});
    attributes1->setParent(measure1.get());
    attributes1->setStaves(dom::presentOptional(1));
    attributes1->setStart(0);

    auto key = std::unique_ptr<dom::Key>(new dom::Key{});
    key->setFifths(2);
    attributes1->setKey(1, std::move(key));

    measure1->addNode(std::move(attributes1));
    manager.addAllAttributes(*measure1);

    manager.addAlter(kMeasureIndex, 1, kStaff, kOctave, dom::Pitch::STEP_C, 0);
    manager.addAlter(kMeasureIndex, 2, kStaff, kOctave, dom::Pitch::STEP_D, 1);

    // Note C is initially 1 from the key signature and gets modified at time 1 back to 0
    BOOST_CHECK_EQUAL(manager.alter(kMeasureIndex, 0, kStaff, kOctave, dom::Pitch::STEP_C), 1);
    BOOST_CHECK_EQUAL(manager.alter(kMeasureIndex, 1, kStaff, kOctave, dom::Pitch::STEP_C), 0);
    BOOST_CHECK_EQUAL(manager.alter(kMeasureIndex, 2, kStaff, kOctave, dom::Pitch::STEP_C), 0);

    // Note D is initially 0 and gets modified at time 2
    BOOST_CHECK_EQUAL(manager.alter(kMeasureIndex, 0, kStaff, kOctave, dom::Pitch::STEP_D), 0);
    BOOST_CHECK_EQUAL(manager.alter(kMeasureIndex, 1, kStaff, kOctave, dom::Pitch::STEP_D), 0);
    BOOST_CHECK_EQUAL(manager.alter(kMeasureIndex, 2, kStaff, kOctave, dom::Pitch::STEP_D), 1);

    // Key signature modifications should not be preserved across measures
    BOOST_CHECK_EQUAL(manager.alter(kMeasureIndex + 1, 0, kStaff, kOctave, dom::Pitch::STEP_C), 1);
    BOOST_CHECK_EQUAL(manager.alter(kMeasureIndex + 1, 0, kStaff, kOctave, dom::Pitch::STEP_D), 0);
}
