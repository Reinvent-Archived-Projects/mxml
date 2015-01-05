//  Created by Alejandro Isaza on 2014-05-12.
//  Copyright (c) 2014 Venture Media Labs. All rights reserved.

#include "SpanDirectionGeometryFactory.h"
#include "MeasureGeometry.h"

#include <mxml/Metrics.h>

namespace mxml {

SpanDirectionGeometryFactory::SpanDirectionGeometryFactory(const PartGeometry& partGeometry) : _partGeometry(partGeometry) {
    
}

SpanDirectionGeometryFactory::CollectionType&& SpanDirectionGeometryFactory::buildTieGeometries(const CollectionType& geometries) {
    _geometries.clear();
    _startGeometries.clear();
    _origin = {0, -Metrics::stavesHeight(_partGeometry.part())/2};
    createGeometries(geometries);
    return std::move(_geometries);
}

void SpanDirectionGeometryFactory::createGeometries(const CollectionType& geometries) {
    for (auto& geom : geometries) {
        if (auto measure = dynamic_cast<MeasureGeometry*>(geom.get())) {
            _origin.x = measure->origin().x;
            createGeometries(measure->geometries());
        } else if (auto direction = dynamic_cast<SpanDirectionGeometry*>(geom.get())) {
            createGeometryFromDirection(*direction);
        }
    }
}

void SpanDirectionGeometryFactory::createGeometryFromDirection(const SpanDirectionGeometry& geometry) {
    
}

} // namespace mxml
