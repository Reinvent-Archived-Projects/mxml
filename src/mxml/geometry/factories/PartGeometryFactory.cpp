// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "EndingGeometryFactory.h"
#include "LyricGeometryFactory.h"
#include "MeasureGeometryFactory.h"
#include "OrnamentGeometryFactory.h"
#include "PartGeometryFactory.h"
#include "TieGeometryFactory.h"

#include <mxml/geometry/collisions/CollisionHandler.h>


namespace mxml {

PartGeometryFactory::PartGeometryFactory(const dom::Part& part, const ScoreProperties& scoreProperties, const Metrics& metrics, const SpanCollection& spans, DirectionGeometryFactory& directionGeometryFactory)
: _part(part),
  _scoreProperties(scoreProperties),
  _metrics(metrics),
  _spans(spans),
  _partGeometry(),
  _directionGeometryFactory(directionGeometryFactory)
{
}

PartGeometryFactory::~PartGeometryFactory() {
    assert(!_partGeometry);
}

std::unique_ptr<PartGeometry> PartGeometryFactory::build() {
    return build(0, _scoreProperties.measureCount());
}

std::unique_ptr<PartGeometry> PartGeometryFactory::build(std::size_t beginMeasure, std::size_t endMeasure){
    _partGeometry.reset(new PartGeometry(_part, _scoreProperties, _metrics));

    MeasureGeometryFactory mesureGeometryFactory(_spans, _scoreProperties, _metrics);

    coord_t offset = 0;
    for (std::size_t measureIndex = beginMeasure; measureIndex != endMeasure; measureIndex += 1) {
        auto& measure = _part.measures()[measureIndex];

        auto geo = mesureGeometryFactory.build(*measure, measureIndex == beginMeasure);
        geo->setHorizontalAnchorPointValues(0, 0);
        geo->setVerticalAnchorPointValues(0, -geo->contentOffset().y);
        geo->setLocation({offset, 0});
        offset += geo->size().width;

        _partGeometry->_measureGeometries.push_back(geo.get());
        _partGeometry->addGeometry(std::move(geo));
    }

    // We need the bounds to match the measures
    auto bounds = _partGeometry->subGeometriesFrame();
    bounds.origin.x = 0;
    _partGeometry->setBounds(bounds);

    _directionGeometryFactory.reset(_partGeometry.get(), _partGeometry->measureGeometries(), _metrics);
    auto directions = _directionGeometryFactory.build();
    for (auto& geometry : directions) {
        _partGeometry->directionGeometries().push_back(geometry.get());
        _partGeometry->addGeometry(std::move(geometry));
    }

    OrnamentGeometryFactory ornamentGeometryFactory(_partGeometry.get(), _partGeometry->measureGeometries(), _metrics);
    auto ornaments = ornamentGeometryFactory.build();
    for (auto& geometry : ornaments) {
        _partGeometry->directionGeometries().push_back(geometry.get());
        _partGeometry->addGeometry(std::move(geometry));
    }

    EndingGeometryFactory endingnGeometryFactory(_partGeometry->measureGeometries(), _metrics);
    auto endings = endingnGeometryFactory.build();
    for (auto& geometry : endings) {
        _partGeometry->directionGeometries().push_back(geometry.get());
        _partGeometry->addGeometry(std::move(geometry));
    }

    LyricGeometryFactory lyricGeometryFactory(*_partGeometry, _partGeometry->measureGeometries(), _metrics);
    auto lyrics = lyricGeometryFactory.build();
    for (auto& lyric : lyrics) {
        _partGeometry->directionGeometries().push_back(lyric.get());
        _partGeometry->addGeometry(std::move(lyric));
    }

    TieGeometryFactory factory(*_partGeometry, _metrics);
    auto ties = factory.buildTieGeometries(_partGeometry->geometries());
    for (auto& tie : ties) {
        _partGeometry->_tieGeometries.push_back(tie.get());
        _partGeometry->addGeometry(std::move(tie));
    }
    
    CollisionHandler collisionHandler(*_partGeometry, _metrics);
    collisionHandler.resolveCollisions();

    // Re-compute bounds after evertyhing is done
    for (auto measure : _partGeometry->measureGeometries())
        mesureGeometryFactory.adjustBounds(measure);
    bounds = _partGeometry->subGeometriesFrame();
    bounds.origin.x = 0;
    _partGeometry->setBounds(bounds);

    return std::move(_partGeometry);
}

} // namespace mxml
