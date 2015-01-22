//  Created by Alejandro Isaza on 2014-06-26.
//  Copyright (c) 2014 Venture Media Labs. All rights reserved.

#include "PartGeometry.h"
#include "PartGeometryFactory.h"
#include "ScoreGeometry.h"
#include <mxml/SpanFactory.h>

namespace mxml {

ScoreGeometry::ScoreGeometry(const dom::Score& score, const ScoreProperties& scoreProperties, const ScrollMetrics& metrics, bool naturalSpacing)
: _score(score),
  _scoreProperties(scoreProperties),
  _metrics(metrics)
{
    SpanFactory spanFactory(_score, _scoreProperties, naturalSpacing);
    _spans = spanFactory.build();

    coord_t offset = 0;
    for (auto& part : _score.parts()) {
        PartGeometryFactory factory(*part, _scoreProperties, _metrics, _spans);
        std::unique_ptr<PartGeometry> geom = factory.build();
        geom->setHorizontalAnchorPointValues(0, 0);
        geom->setVerticalAnchorPointValues(0, 0);
        geom->setLocation({0, offset});
        offset += geom->size().height;
        _partGeometries.push_back(geom.get());
        addGeometry(std::move(geom));
    }
    
    setBounds(subGeometriesFrame());
}

} // namespace
