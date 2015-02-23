//  Created by Alejandro Isaza on 2015-01-23.
//  Copyright (c) 2015 Venture Media Labs. All rights reserved.

#include "PageScoreGeometry.h"
#include <mxml/SpanFactory.h>


namespace mxml {

const coord_t kSystemDistancePadding = 20;

PageScoreGeometry::PageScoreGeometry(const dom::Score& score, coord_t minWidth)
: _score(score),
  _scoreProperties(score, ScoreProperties::kLayoutTypePage)
{
    SpanFactory spanFactory(_score, _scoreProperties);
    spanFactory.setNaturalSpacing(false);
    _spans = spanFactory.build();

    // Make all widths uniform
    const auto width = std::max(minWidth, maxSystemWidth());
    for (std::size_t systemIndex = 0; systemIndex < _scoreProperties.systemCount(); systemIndex += 1) {
        auto range = _scoreProperties.measureRange(systemIndex);
        _spans->fitToWidth(width, range.first, range.second);
    }
    _spans->fillStarts();

    // Create system geometires
    for (std::size_t systemIndex = 0; systemIndex < _scoreProperties.systemCount(); systemIndex += 1) {
        auto systemGeometry = std::unique_ptr<SystemGeometry>(new SystemGeometry(_score, _scoreProperties, *_spans, systemIndex, width));
        systemGeometry->setHorizontalAnchorPointValues(0, 0);
        systemGeometry->setVerticalAnchorPointValues(0, 0);
        _systemGeometries.push_back(systemGeometry.get());
        addGeometry(std::move(systemGeometry));
    }

    // Make all distances uniform
    const auto distance = maxSystemDistance();
    setSystemDistances(distance);

    // Force the content offset in x and width so that the page aligns to the screen
    auto bounds = subGeometriesFrame();
    bounds.origin.x = 0;
    bounds.size.width = width;
    setBounds(bounds);
}

coord_t PageScoreGeometry::maxSystemWidth() const {
    coord_t width = 0;
    for (std::size_t systemIndex = 0; systemIndex < _scoreProperties.systemCount(); systemIndex += 1) {
        auto range = _scoreProperties.measureRange(systemIndex);
        coord_t systemWidth = 0;
        for (auto measureIndex = range.first; measureIndex != range.second; measureIndex += 1) {
            systemWidth += _spans->width(measureIndex);
        }

        if (systemWidth > width)
            width = systemWidth;
    }
    return width;
}

coord_t PageScoreGeometry::maxSystemDistance() const {
    coord_t prevBottom = 0;
    coord_t prevBottomPadding = 0;
    coord_t maxSystemDistance = 0;
    for (auto systemGeometry : _systemGeometries) {
        if (prevBottom > 0) {
            const auto topPadding = systemGeometry->topPadding();
            const auto prevStavesBottom = prevBottom - prevBottomPadding;
            const auto currentStavesTop = prevBottom + topPadding;
            const auto systemDistance = currentStavesTop - prevStavesBottom;
            maxSystemDistance = std::max(maxSystemDistance, systemDistance);
        }
        prevBottom = systemGeometry->frame().max().y;
        prevBottomPadding = systemGeometry->bottomPadding();
    }
    return maxSystemDistance + kSystemDistancePadding;
}

void PageScoreGeometry::setSystemDistances(const coord_t distance) {
    coord_t prevBottom = 0;
    coord_t prevBottomPadding = 0;
    for (auto systemGeometry : _systemGeometries) {
        if (prevBottom > 0) {
            const auto topPadding = systemGeometry->topPadding();
            systemGeometry->setLocation({0, prevBottom - prevBottomPadding + distance - topPadding});
        }
        prevBottom = systemGeometry->frame().max().y;
        prevBottomPadding = systemGeometry->bottomPadding();
    }
}

void PageScoreGeometry::setActiveRange(std::size_t startMeasureIndex, std::size_t endMeasureIndex) {
    for (auto& systemGeometry : _systemGeometries) {
        systemGeometry->setActiveRange(startMeasureIndex, endMeasureIndex);
    }
}

} // namespace mxml
