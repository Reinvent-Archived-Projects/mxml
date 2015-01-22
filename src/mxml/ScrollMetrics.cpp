//  Copyright (c) 2014 Venture Media Labs. All rights reserved.

#include "ScrollMetrics.h"

namespace mxml {

ScrollMetrics::ScrollMetrics(const dom::Score& score, const ScoreProperties& scoreProperties, std::size_t partIndex)
: Metrics(score, scoreProperties, partIndex)
{
}

dom::tenths_t ScrollMetrics::staffDistance() const {
    dom::tenths_t distance = 65;

    // Get default staff distance from the `defaults` element
    auto& defaults = _score.defaults();
    if (defaults) {
        if (defaults->staffDistance())
            distance = defaults->staffDistance();
    }

    // We should consider finding the maximal distance from all the `print` elements, but this will sometimes make the
    // space between the staves too large

    return distance;
}

}
