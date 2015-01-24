//  Copyright (c) 2015 Venture Media Labs. All rights reserved.

#include "SystemGeometry.h"

#include <mxml/geometry/collisions/CollisionHandler.h>
#include <mxml/geometry/factories/PartGeometryFactory.h>
#include <mxml/SpanFactory.h>

namespace mxml {

SystemGeometry::SystemGeometry(const dom::Score& score, const ScoreProperties& scoreProperties, std::size_t systemIndex)
: _score(score),
  _scoreProperties(scoreProperties),
  _systemIndex(systemIndex)
{
    SpanFactory spanFactory(_score, _scoreProperties, false);
    _spans = spanFactory.build();

    auto range = _scoreProperties.measureRange(systemIndex);

    std::size_t partIndex = 0;
    coord_t offset = 0;
    for (auto& part : _score.parts()) {
        _metrics.emplace_back(new PageMetrics(_score, _scoreProperties, systemIndex, partIndex));

        PartGeometryFactory factory(*part, _scoreProperties, *_metrics.back(), _spans);
        std::unique_ptr<PartGeometry> geom = factory.build(range.first, range.second);
        geom->setHorizontalAnchorPointValues(0, 0);
        geom->setVerticalAnchorPointValues(0, 0);
        geom->setLocation({0, offset});
        offset += geom->size().height;
        _partGeometries.push_back(geom.get());
        addGeometry(std::move(geom));

        partIndex += 1;
    }

    setBounds(subGeometriesFrame());
}

} // namespace
