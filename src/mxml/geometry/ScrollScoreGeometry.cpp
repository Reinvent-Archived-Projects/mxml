// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "ScrollScoreGeometry.h"
#include <mxml/geometry/factories/PartGeometryFactory.h>
#include <mxml/SpanFactory.h>

namespace mxml {

ScrollScoreGeometry::ScrollScoreGeometry(const dom::Score& score, bool naturalSpacing)
: _score(score),
  _scoreProperties(score, ScoreProperties::LayoutType::Scroll),
  _spans()
{
    SpanFactory spanFactory(_score, _scoreProperties);
    spanFactory.setNaturalSpacing(naturalSpacing);
    _spans = spanFactory.build();

    DirectionGeometryFactory directionGeometryFactory;

    std::size_t partIndex = 0;
    coord_t offset = 0;
    for (auto& part : _score.parts()) {
        _metrics.emplace_back(new ScrollMetrics(_score, _scoreProperties, partIndex));

        PartGeometryFactory factory(*part, _scoreProperties, *_metrics.back(), *_spans, directionGeometryFactory);
        std::unique_ptr<PartGeometry> geom = factory.build();
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

void ScrollScoreGeometry::setActiveRange(std::size_t startMeasureIndex, std::size_t endMeasureIndex) {
    for (auto& partGeometry: _partGeometries) {
        partGeometry->setActiveRange(startMeasureIndex, endMeasureIndex);
    }
}

} // namespace mxml
