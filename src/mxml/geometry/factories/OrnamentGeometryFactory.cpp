//  Created by Alejandro Isaza on 2015-01-22.
//  Copyright (c) 2015 Venture Media Labs. All rights reserved.

#include "OrnamentGeometryFactory.h"
#include "MeasureGeometry.h"

#include <mxml/geometry/ChordGeometry.h>


namespace mxml {

OrnamentGeometryFactory::OrnamentGeometryFactory(const Geometry* parentGeometry, const std::vector<MeasureGeometry*>& measureGeometries, const Metrics& metrics)
: _parentGeometry(parentGeometry),
  _measureGeometries(measureGeometries),
  _metrics(metrics)
{
}

std::vector<std::unique_ptr<OrnamentsGeometry>> OrnamentGeometryFactory::build() {
    _geometries.clear();

    for (auto& measure: _measureGeometries) {
        for (auto& geom : measure->geometries()) {
            if (const ChordGeometry* chord = dynamic_cast<const ChordGeometry*>(geom.get()))
                buildOrnaments(*measure, *chord);
        }
    }
    return std::move(_geometries);
}

void OrnamentGeometryFactory::buildOrnaments(const MeasureGeometry& measureGeom, const ChordGeometry& chordGeom) {
    for (auto& note : chordGeom.chord().notes()) {
        if (!note->notations)
            continue;

        auto& notations = note->notations;
        for (auto& ornament : notations->ornaments)
            buildOrnament(measureGeom, chordGeom, *ornament);
    }
}

void OrnamentGeometryFactory::buildOrnament(const MeasureGeometry& measureGeom, const ChordGeometry& chordGeom, const dom::Ornaments& ornaments) {
    const dom::EmptyPlacement* placement = ornaments.placement();
    if (!placement)
        return;

    const int staff = chordGeom.chord().firstNote()->staff();
    std::unique_ptr<OrnamentsGeometry> geo(new OrnamentsGeometry(ornaments, staff));

    const Span& span = *measureGeom.spans().with(&chordGeom.chord());
    Point location;
    location.x = span.start() + span.eventOffset();
    geo->setLocation(location);

    place(*geo);

    // Avoid placing on top of existing geometries
    Rect frame = _parentGeometry->convertToGeometry(geo->frame(), &measureGeom);
    auto collidingGeom = measureGeom.collidingGeometry(frame);
    if (collidingGeom) {
        if (placement->placement() == dom::Placement::Above) {
            Point origin = _parentGeometry->convertFromGeometry(collidingGeom->origin(), &measureGeom);
            location.y = origin.y - geo->size().height/2 - 4;
            geo->setLocation(location);
        } else {
            Point point = _parentGeometry->convertFromGeometry(collidingGeom->frame().max(), &measureGeom);
            location.y = point.y + geo->size().height/2 + 4;
            geo->setLocation(location);
        }
    }
    
    _geometries.push_back(std::move(geo));
}

void OrnamentGeometryFactory::place(PlacementGeometry& geometry) {
    auto location = geometry.location();

    if (geometry.placement() == dom::Placement::Above) {
        location.y = _metrics.staffOrigin(geometry.staff()) - Metrics::kStaffLineSpacing;
        geometry.setVerticalAnchorPointValues(1, 0);
    } else {
        location.y = _metrics.staffOrigin(geometry.staff()) + Metrics::staffHeight() + Metrics::kStaffLineSpacing;
        geometry.setVerticalAnchorPointValues(0, 0);
    }
    
    geometry.setLocation(location);
}

} // namespace mxml
