// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "SystemGeometry.h"

#include <mxml/geometry/collisions/CollisionHandler.h>
#include <mxml/geometry/factories/PartGeometryFactory.h>


namespace mxml {

SystemGeometry::SystemGeometry(const dom::Score& score, const ScoreProperties& scoreProperties, const SpanCollection& spans, DirectionGeometryFactory& directionGeometryFactory, std::size_t systemIndex, coord_t width)
: _score(score),
  _scoreProperties(scoreProperties),
  _spans(spans),
  _systemIndex(systemIndex),
  _directionGeometryFactory(directionGeometryFactory)
{
    auto range = _scoreProperties.measureRange(systemIndex);

    std::size_t partIndex = 0;
    coord_t offset = 0;
    for (auto& part : _score.parts()) {
        _metrics.emplace_back(new PageMetrics(_score, _scoreProperties, systemIndex, partIndex));

        PartGeometryFactory factory(*part, _scoreProperties, *_metrics.back(), _spans, _directionGeometryFactory);
        std::unique_ptr<PartGeometry> geom = factory.build(range.first, range.second);

        geom->setHorizontalAnchorPointValues(0, 0);
        geom->setVerticalAnchorPointValues(0, 0);
        geom->setLocation({0, offset});

        // Force the width so that everything aligns
        auto size = geom->size();
        size.width = width;
        geom->setSize(size);

        offset += geom->size().height;
        _partGeometries.push_back(geom.get());
        addGeometry(std::move(geom));

        partIndex += 1;
    }

    // Force the content offset in x and width so that all systems align properly
    auto bounds = subGeometriesFrame();
    bounds.origin.x = 0;
    bounds.size.width = width;
    setBounds(bounds);
}

coord_t SystemGeometry::topPadding() const {
    if (_partGeometries.empty())
        return {};

    auto& firstPart = _partGeometries.front();
    auto& firstMeasure = firstPart->measureGeometries().front();
    return firstMeasure->convertToGeometry({0, 0}, this).y;
}

coord_t SystemGeometry::bottomPadding() const {
    if (_partGeometries.empty())
        return {};

    auto& metrics = _metrics.back();
    auto& lastPart = _partGeometries.back();
    auto& firstMeasure = lastPart->measureGeometries().front();
    return bounds().max().y - firstMeasure->convertToGeometry({0, metrics->stavesHeight()}, this).y;
}

void SystemGeometry::setActiveRange(std::size_t startMeasureIndex, std::size_t endMeasureIndex) {
    auto systemRange = _scoreProperties.measureRange(_systemIndex);
    if (startMeasureIndex <= systemRange.first && endMeasureIndex >= systemRange.second) {
        setActive(true);
    } else if (startMeasureIndex >= systemRange.second || endMeasureIndex <= systemRange.first) {
        setActive(false);
    } else {
        setActive(true);
        for (auto& partGeometry: _partGeometries) {
            partGeometry->setActiveRange(startMeasureIndex, endMeasureIndex);
        }
    }
}

} // namespace
