// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <mxml/EqualityConstraintSolver.h>
#include <boost/test/unit_test.hpp>
#include <iostream>

using namespace mxml;

BOOST_AUTO_TEST_CASE(trivialCase) {
    EqualityConstraintSolver solver;
    solver.setVariableCount(1);
    auto count = solver.solve([](const std::vector<bool>& values) {
        BOOST_CHECK_EQUAL(values.size(), 1);
    });
    BOOST_CHECK_EQUAL(count, 2);
}

BOOST_AUTO_TEST_CASE(unsatisfyable) {
    EqualityConstraintSolver solver;
    solver.setVariableCount(3);
    solver.addDifferentConstraint(0, 1);
    solver.addDifferentConstraint(1, 2);
    solver.addDifferentConstraint(0, 2);

    auto count = solver.solve([](const std::vector<bool>& values) {
        BOOST_FAIL("There should not be a solution");
    });
    BOOST_CHECK_EQUAL(count, 0);
}

BOOST_AUTO_TEST_CASE(complex) {
    EqualityConstraintSolver solver;
    solver.setVariableCount(8);

    // A complex system with two beams and two solo notes:
    //
    //  0    1    2     7
    //  ┌────┬────┐
    //  │    │    │     │
    // ●    ●    ●     ●     ●
    //     │    │     │     │
    //          └─────┴─────┘
    //     3    4    5      6
    //

    // Beam: 0, 1, 2
    solver.addEqualConstraint(0, 1);
    solver.addEqualConstraint(1, 2);

    // Solo note: 3
    solver.addDifferentConstraint(1, 3);

    // Beam: 4, 5, 6
    solver.addEqualConstraint(4, 5);
    solver.addEqualConstraint(5, 6);
    solver.addEqualConstraint(4, 6);

    // Solo note: 7
    solver.addDifferentConstraint(5, 7);

    // Beam contact
    solver.addDifferentConstraint(2, 4);

    auto count = solver.solve([](const std::vector<bool>& values) {
        BOOST_CHECK_EQUAL(values[0], values[1]);
        BOOST_CHECK_EQUAL(values[1], values[2]);
        BOOST_CHECK_EQUAL(values[4], values[5]);
        BOOST_CHECK_EQUAL(values[5], values[6]);
        BOOST_CHECK_NE(values[1], values[3]);
        BOOST_CHECK_NE(values[2], values[4]);
        BOOST_CHECK_NE(values[5], values[7]);
    });

    // There should be two solutons: the one shown and it's opposite
    BOOST_CHECK_EQUAL(count, 2);
}
