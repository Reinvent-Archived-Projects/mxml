//  Copyright (c) 2014 Venture Media Labs. All rights reserved.

#include "ScrollMetrics.h"

namespace mxml {

ScrollMetrics::ScrollMetrics(const dom::Score& score, const ScoreProperties& scoreProperties)
: Metrics(score, scoreProperties)
{
}

dom::tenths_t ScrollMetrics::staffDistance(std::size_t partIndex) const {
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

dom::tenths_t ScrollMetrics::stavesHeight(std::size_t partIndex) const {
    return Metrics::stavesHeight(_scoreProperties.staves(partIndex), staffDistance(partIndex));
}

dom::tenths_t ScrollMetrics::staffOrigin(std::size_t partIndex, int staffNumber) const {
    return (staffNumber - 1) * (staffHeight() + staffDistance(partIndex));
}

dom::tenths_t ScrollMetrics::noteY(const dom::Note& note) const {
    const auto& part = static_cast<const dom::Part&>(*note.measure()->parent());
    return staffOrigin(part.index(), note.staff()) + staffY(note);
}

}
