//  Created by Alejandro Isaza on 2014-06-26.
//  Copyright (c) 2014 Venture Media Labs. All rights reserved.

#include "PartGeometry.h"
#include "ScoreGeometry.h"
#include <mxml/geometry/factories/PartGeometryFactory.h>
#include <mxml/SpanFactory.h>

namespace mxml {

ScoreGeometry::ScoreGeometry(const dom::Score& score, const ScoreProperties& scoreProperties, bool naturalSpacing)
: _score(score),
  _scoreProperties(scoreProperties)
{
    SpanFactory spanFactory(_score, _scoreProperties, naturalSpacing);
    _spans = spanFactory.build();

    std::size_t partIndex = 0;
    coord_t offset = 0;
    for (auto& part : _score.parts()) {
        _metrics.emplace_back(new ScrollMetrics(_score, _scoreProperties, partIndex));

        PartGeometryFactory factory(*part, _scoreProperties, *_metrics.back(), _spans);
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

} // namespace
