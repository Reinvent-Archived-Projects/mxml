//  Created by Alejandro Isaza on 2015-01-05.
//  Copyright (c) 2015 Venture Media Labs. All rights reserved.

#include "ChordGeometry.h"
#include "LyricGeometry.h"
#include "LyricGeometryFactory.h"
#include "MeasureGeometry.h"

#include <mxml/Metrics.h>

namespace mxml {

LyricGeometryFactory::LyricGeometryFactory(const Geometry& parent, const std::vector<MeasureGeometry*>& measureGeometries, const Metrics& metrics)
: _parent(parent),
  _measureGeometries(measureGeometries),
  _metrics(metrics)
{}

std::vector<std::unique_ptr<LyricGeometry>> LyricGeometryFactory::build() {
    _lyricGeometries.clear();
    computeNotesBounds();
    
    for (auto& measure: _measureGeometries) {
        for (auto& geom : measure->geometries()) {
            if (const ChordGeometry* chord = dynamic_cast<const ChordGeometry*>(geom.get()))
                build(*measure, *chord);
        }
    }

    return std::move(_lyricGeometries);
}

void LyricGeometryFactory::computeNotesBounds() {
    _chordsBounds.origin.x = 0;
    _chordsBounds.origin.y = -Metrics::kStaffLineSpacing;
    _chordsBounds.size.width = 0;
    _chordsBounds.size.height = Metrics::staffHeight() + Metrics::kStaffLineSpacing;

    // Get the bounding box of all notes on this part to place lyrics below that
    for (auto& measure: _measureGeometries) {
        for (auto& geom : measure->geometries()) {
            auto chord = dynamic_cast<ChordGeometry*>(geom.get());
            if (!chord)
                continue;

            _chordsBounds = join(_chordsBounds, chord->frame());
        }
    }
}

void LyricGeometryFactory::build(const MeasureGeometry& measureGeom, const ChordGeometry& chordGeom) {
    for (auto& noteGeom : chordGeom.notes()) {
        for (auto& lyric : noteGeom->note().lyrics()) {
            build(measureGeom, chordGeom, *lyric);
        }
    }
}

void LyricGeometryFactory::build(const MeasureGeometry& measureGeom, const ChordGeometry& chordGeom, const dom::Lyric& lyric) {
    const int staff = chordGeom.chord().firstNote()->staff();
    std::unique_ptr<LyricGeometry> geometry(new LyricGeometry(lyric, staff));

    auto& spans = measureGeom.spans();
    auto measureIndex = measureGeom.measure().index();
    auto measureOrigin = spans.origin(measureIndex);

    const Span& span = *measureGeom.spans().with(&chordGeom.chord());
    Point location;
    location.x = span.start() + span.eventOffset() - measureOrigin;
    location = _parent.convertFromGeometry(location, &measureGeom);

    if (geometry->placement() == dom::kPlacementAbove) {
        location.y = _chordsBounds.min().y;
        geometry->setVerticalAnchorPointValues(1, 0);
    } else {
        location.y = _chordsBounds.max().y;
        geometry->setVerticalAnchorPointValues(0, 0);
    }

    if (lyric.number() > 1) {
        if (geometry->placement() == dom::kPlacementAbove)
            location.y -= (lyric.number() - 1) * geometry->size().height;
        else
            location.y += (lyric.number() - 1) * geometry->size().height;
    }

    geometry->setLocation(location);

    _lyricGeometries.push_back(std::move(geometry));
}

} // namespace mxml
