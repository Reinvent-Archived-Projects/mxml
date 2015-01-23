//  Copyright (c) 2015 Venture Media Labs. All rights reserved.

#include "DirectionGeometryFactory.h"
#include "EndingGeometryFactory.h"
#include "LyricGeometryFactory.h"
#include "OrnamentGeometryFactory.h"
#include "SystemGeometryFactory.h"
#include "TieGeometryFactory.h"

#include <mxml/geometry/collisions/CollisionHandler.h>


namespace mxml {

SystemGeometryFactory::SystemGeometryFactory(const dom::Part& part, const ScoreProperties& scoreProperties, const PageMetrics& metrics, const SpanCollection& spans)
: _part(part),
  _scoreProperties(scoreProperties),
  _metrics(metrics),
  _spans(spans)
{
}

SystemGeometryFactory::~SystemGeometryFactory() {
    assert(!_systemGeometry);
}

std::unique_ptr<SystemGeometry> SystemGeometryFactory::build(const System& system) {
    _systemGeometry.reset(new SystemGeometry(_part, system, _scoreProperties, _metrics));

    coord_t offset = 0;
    for (std::size_t measureIndex = system.measureBegin; measureIndex != system.measureEnd; measureIndex += 1) {
        auto& measure = _part.measures()[measureIndex];

        std::unique_ptr<MeasureGeometry> geo(new MeasureGeometry(*measure, _spans, _scoreProperties, _metrics));
        geo->setHorizontalAnchorPointValues(0, 0);
        geo->setLocation({offset, 0});
        offset += geo->size().width;

        _systemGeometry->_measureGeometries.push_back(geo.get());
        _systemGeometry->addGeometry(std::move(geo));
    }

    DirectionGeometryFactory directionGeometryFactory(_systemGeometry.get(), _systemGeometry->measureGeometries(), _metrics);
    auto directions = directionGeometryFactory.build();
    for (auto& geometry : directions) {
        _systemGeometry->directionGeometries().push_back(geometry.get());
        _systemGeometry->addGeometry(std::move(geometry));
    }

    OrnamentGeometryFactory ornamentGeometryFactory(_systemGeometry.get(), _systemGeometry->measureGeometries(), _metrics);
    auto ornaments = ornamentGeometryFactory.build();
    for (auto& geometry : ornaments) {
        _systemGeometry->directionGeometries().push_back(geometry.get());
        _systemGeometry->addGeometry(std::move(geometry));
    }

    EndingGeometryFactory endingnGeometryFactory(_systemGeometry->measureGeometries(), _metrics);
    auto endings = endingnGeometryFactory.build();
    for (auto& geometry : endings) {
        _systemGeometry->directionGeometries().push_back(geometry.get());
        _systemGeometry->addGeometry(std::move(geometry));
    }

    LyricGeometryFactory lyricGeometryFactory(_systemGeometry->measureGeometries(), _metrics);
    auto lyrics = lyricGeometryFactory.build();
    for (auto& lyric : lyrics) {
        _systemGeometry->directionGeometries().push_back(lyric.get());
        _systemGeometry->addGeometry(std::move(lyric));
    }

    TieGeometryFactory factory(*_systemGeometry, _metrics);
    auto ties = factory.buildTieGeometries(_systemGeometry->geometries());
    for (auto& tie : ties) {
        _systemGeometry->_tieGeometries.push_back(tie.get());
        _systemGeometry->addGeometry(std::move(tie));
    }

    CollisionHandler collisionHandler(*_systemGeometry, _metrics);
    collisionHandler.resolveCollisions();

    _systemGeometry->setBounds(_systemGeometry->subGeometriesFrame());

    return std::move(_systemGeometry);
}

} // namespace
