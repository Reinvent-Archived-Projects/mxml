//  Copyright (c) 2015 Venture Media Labs. All rights reserved.

#include "SystemGeometry.h"

#include <mxml/geometry/collisions/CollisionHandler.h>
#include <mxml/geometry/factories/PartGeometryFactory.h>
#include <mxml/SpanFactory.h>

namespace mxml {

SystemGeometry::SystemGeometry(const dom::Score& score, const ScoreProperties& scoreProperties, std::size_t systemIndex, coord_t width)
: _score(score),
  _scoreProperties(scoreProperties),
  _systemIndex(systemIndex)
{
    auto range = _scoreProperties.measureRange(systemIndex);

    SpanFactory spanFactory(_score, _scoreProperties, false);
    _spans = spanFactory.build(range.first, range.second, width);

    std::size_t partIndex = 0;
    coord_t offset = 0;
    for (auto& part : _score.parts()) {
        _metrics.emplace_back(new PageMetrics(_score, _scoreProperties, systemIndex, partIndex));

        PartGeometryFactory factory(*part, _scoreProperties, *_metrics.back(), _spans);
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

} // namespace
