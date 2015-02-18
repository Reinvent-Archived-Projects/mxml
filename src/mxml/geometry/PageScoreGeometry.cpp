//  Created by Alejandro Isaza on 2015-01-23.
//  Copyright (c) 2015 Venture Media Labs. All rights reserved.

#include "PageScoreGeometry.h"
#include <mxml/SpanFactory.h>


namespace mxml {

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

    coord_t prevBottom = 0;
    coord_t prevBottomPadding = 0;
    for (std::size_t systemIndex = 0; systemIndex < _scoreProperties.systemCount(); systemIndex += 1) {
        auto systemGeometry = std::unique_ptr<SystemGeometry>(new SystemGeometry(_score, _scoreProperties, *_spans, systemIndex, width));
        auto& metrics = systemGeometry->metrics(_score.parts().size() - 1);

        const auto systemDistance = metrics.systemDistance();
        const auto topPadding = systemGeometry->topPadding();
        auto top = prevBottom;
        if (prevBottom > 0) {
            top = std::max(top, prevBottom - prevBottomPadding + systemDistance - topPadding);
        }

        systemGeometry->setHorizontalAnchorPointValues(0, 0);
        systemGeometry->setVerticalAnchorPointValues(0, 0);
        systemGeometry->setLocation({0, top});

        prevBottom = systemGeometry->frame().max().y;
        prevBottomPadding = systemGeometry->bottomPadding();

        _systemGeometries.push_back(systemGeometry.get());
        addGeometry(std::move(systemGeometry));
    }

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

void PageScoreGeometry::setActiveRange(std::size_t startMeasureIndex, std::size_t endMeasureIndex) {
    for (auto& systemGeometry : _systemGeometries) {
        systemGeometry->setActiveRange(startMeasureIndex, endMeasureIndex);
    }
}

} // namespace mxml
