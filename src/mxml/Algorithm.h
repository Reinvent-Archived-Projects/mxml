// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once
#include <algorithm>
#include <cmath>
#include <functional>
#include <numeric>


namespace mxml {

/**
 Given a sequence of real values `A` and its integer sum `T`, find the set of integers `B` such that the sum of all
 elements of `B` equals `T` and the difference between each value `e = |Ai - Bi|` is minimized. The difference `e` will
 never be greater than 1.

 @param begin  The begin iterator to the sequence of real values 'A'
 @param end    The end iterator to the sequence of real values 'A'
 @param output The output iterator to the sequence of integer values 'B'
 */
template<typename InputIterator, typename OutputIterator>
void integerSum(InputIterator begin, const InputIterator end, OutputIterator output) {
    auto error = typename std::iterator_traits<InputIterator>::value_type{0};
    while (begin != end) {
        const auto real = *begin;
        const auto floor = std::floor(real);
        const auto floorError = error + (floor - real);
        const auto ceil = std::ceil(real);
        const auto ceilError = error + (ceil - real);

        if (std::abs(floorError) < std::abs(ceilError)) {
            *output = floor;
            error = floorError;
        } else {
            *output = ceil;
            error = ceilError;
        }

        ++output;
        ++begin;
    }
}

} // namespace mxml
