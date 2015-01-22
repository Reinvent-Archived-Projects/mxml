//  Copyright (c) 2014 Venture Media Labs. All rights reserved.

#include "PageMetrics.h"

namespace mxml {

PageMetrics::PageMetrics(const dom::Score& score, const ScoreProperties& scoreProperties, std::size_t systemIndex, std::size_t partIndex)
: Metrics(score, scoreProperties, partIndex),
  _systemIndex(systemIndex)
{
}

dom::tenths_t PageMetrics::systemDistance() const {
    dom::tenths_t distance = 80;

    // Get default system distance from the `defaults` element
    auto& defaults = _score.defaults();
    if (defaults) {
        if (defaults->systemLayout) {
            auto& systemLayout = defaults->systemLayout.value();
            if (systemLayout.systemDistance)
                distance = systemLayout.systemDistance;
        }
    }

    for (auto& ref : _prints) {
        if (ref.systemIndex < _systemIndex)
            continue;
        if (ref.systemIndex > _systemIndex)
            break;

        auto& print = *ref.print;
        if (print.systemLayout) {
            auto& systemLayout = print.systemLayout.value();
            if (systemLayout.systemDistance)
                distance = systemLayout.systemDistance;
        }
    }

    return distance;
}

dom::tenths_t PageMetrics::staffDistance() const {
    dom::tenths_t distance = 65;

    // Get default staff distance from the `defaults` element
    auto& defaults = _score.defaults();
    if (defaults) {
        if (defaults->staffDistance())
            distance = defaults->staffDistance();
    }

    for (auto& ref : _prints) {
        if (ref.systemIndex < _systemIndex)
            continue;
        if (ref.systemIndex > _systemIndex)
            break;

        auto& print = *ref.print;
        if (print.staffDistance()) {
            auto& systemLayout = print.systemLayout.value();
            if (systemLayout.systemDistance)
                distance = systemLayout.systemDistance;
        }
    }

    return distance;
}

}
