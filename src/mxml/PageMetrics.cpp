//  Copyright (c) 2014 Venture Media Labs. All rights reserved.

#include "PageMetrics.h"

namespace mxml {

using namespace dom;

const dom::tenths_t Metrics::kStaffLineSpacing = 10;
const std::size_t Metrics::kStaffLineCount = 5;

PageMetrics::PageMetrics(const dom::Score& score, const ScoreProperties& scoreProperties)
: Metrics(score, scoreProperties)
{
}

dom::tenths_t PageMetrics::systemDistance(std::size_t systemIndex) const {
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
        if (ref.systemIndex < systemIndex)
            continue;
        if (ref.systemIndex > systemIndex)
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

dom::tenths_t PageMetrics::staffDistance(std::size_t systemIndex, std::size_t partIndex) const {
    dom::tenths_t distance = 65;

    // Get default staff distance from the `defaults` element
    auto& defaults = _score.defaults();
    if (defaults) {
        if (defaults->staffDistance())
            distance = defaults->staffDistance();
    }

    for (auto& ref : _prints) {
        if (ref.systemIndex < systemIndex)
            continue;
        if (ref.systemIndex > systemIndex)
            break;

        if (ref.partIndex != partIndex)
            continue;

        auto& print = *ref.print;
        if (print.staffDistance()) {
            auto& systemLayout = print.systemLayout.value();
            if (systemLayout.systemDistance)
                distance = systemLayout.systemDistance;
        }
    }

    return distance;
}

dom::tenths_t PageMetrics::stavesHeight(std::size_t systemIndex, std::size_t partIndex) const {
    return Metrics::stavesHeight(_scoreProperties.staves(partIndex), staffDistance(systemIndex, partIndex));
}

dom::tenths_t PageMetrics::staffOrigin(std::size_t systemIndex, std::size_t partIndex, int staffNumber) const {
    return (staffNumber - 1) * (staffHeight() + staffDistance(systemIndex, partIndex));
}

dom::tenths_t PageMetrics::noteY(const dom::Note& note) const {
    auto measureIndex = note.measure()->index();
    auto systemIndex = _scoreProperties.systemIndex(measureIndex);
    auto partIndex = note.measure()->part()->index();
    return staffOrigin(systemIndex, partIndex, note.staff()) + staffY(note);
}

}
