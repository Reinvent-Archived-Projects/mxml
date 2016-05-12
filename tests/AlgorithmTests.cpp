// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <mxml/Algorithm.h>
#include <boost/test/unit_test.hpp>

using namespace mxml;

BOOST_AUTO_TEST_CASE(integerSumUnderTest) {
    std::vector<double> reals(6, 2.3333);
    std::vector<int> integers;
    integerSum(reals.begin(), reals.end(), std::inserter(integers, integers.begin()));

    auto sum = std::accumulate(integers.begin(), integers.end(), 0, std::plus<int>());
    BOOST_CHECK_EQUAL(sum, 14);
    for (auto integer : integers)
        BOOST_CHECK(integer == 2 || integer == 3);
}

BOOST_AUTO_TEST_CASE(integerSumOverTest) {
    std::vector<double> reals(20, 2.9);
    std::vector<int> integers;
    integerSum(reals.begin(), reals.end(), std::inserter(integers, integers.begin()));

    auto sum = std::accumulate(integers.begin(), integers.end(), 0, std::plus<int>());
    BOOST_CHECK_EQUAL(sum, 58);
    for (auto integer : integers)
        BOOST_CHECK(integer == 2 || integer == 3);
}

BOOST_AUTO_TEST_CASE(integerSumInaccurateTotalTest) {
    double reals[] = {2.9, 2.9, 2.9};
    int integers[3];
    integerSum(reals, reals + 3, integers);

    auto sum = std::accumulate(integers, integers + 3, 0, std::plus<int>());
    BOOST_CHECK_EQUAL(sum, 9);
    for (auto integer : integers)
        BOOST_CHECK(integer == 2 || integer == 3);
}
