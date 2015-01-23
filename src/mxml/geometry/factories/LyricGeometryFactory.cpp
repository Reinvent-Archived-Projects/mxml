//  Created by Alejandro Isaza on 2015-01-05.
//  Copyright (c) 2015 Venture Media Labs. All rights reserved.

#include "ChordGeometry.h"
#include "LyricGeometry.h"
#include "LyricGeometryFactory.h"

#include <mxml/Metrics.h>

namespace mxml {

LyricGeometryFactory::LyricGeometryFactory(const std::vector<MeasureGeometry*>& measureGeometries, const Metrics& metrics)
: _measureGeometries(measureGeometries),
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
    _notesBounds.origin.x = 0;
    _notesBounds.origin.y = -Metrics::kStaffLineSpacing;
    _notesBounds.size.width = 0;
    _notesBounds.size.height = Metrics::staffHeight() + Metrics::kStaffLineSpacing;

    // Get the bounding box of all notes on this part to place lyrics below that
    for (auto& measure: _measureGeometries) {
        for (auto& geom : measure->geometries()) {
            auto chord = dynamic_cast<ChordGeometry*>(geom.get());
            if (!chord)
                continue;

            auto chordNotesFrame = chord->notesFrame();
            _notesBounds = join(_notesBounds, chordNotesFrame);
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

    const Span& span = *measureGeom.spans().with(&chordGeom.chord());
    Point location;
    location.x = span.start() + span.eventOffset();

    if (geometry->placement() == dom::kPlacementAbove) {
        location.y = _notesBounds.min().y;
        geometry->setVerticalAnchorPointValues(1, 0);
    } else {
        location.y = _notesBounds.max().y;
        geometry->setVerticalAnchorPointValues(0, 0);
    }

    if (lyric.number() > 1) {
        if (geometry->placement() == dom::kPlacementAbove)
            location.y -= (lyric.number() - 1) * geometry->size().height;
        else
            location.y += (lyric.number() - 1) * geometry->size().height;
    }

    location.y -= _metrics.stavesHeight()/2;
    geometry->setLocation(location);

    _lyricGeometries.push_back(std::move(geometry));
}

} // namespace mxml
