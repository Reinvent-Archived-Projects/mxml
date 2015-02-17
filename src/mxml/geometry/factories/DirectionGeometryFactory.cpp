//  Created by Alejandro Isaza on 2015-01-22.
//  Copyright (c) 2015 Venture Media Labs. All rights reserved.

#include "DirectionGeometryFactory.h"

#include "BarlineGeometry.h"
#include "CodaGeometry.h"
#include "EndingGeometry.h"
#include "NoteGeometry.h"
#include "OctaveShiftGeometry.h"
#include "OrnamentsGeometry.h"
#include "PedalGeometry.h"
#include "SegnoGeometry.h"
#include "SpanDirectionGeometry.h"
#include "WordsGeometry.h"

#include <mxml/dom/OctaveShift.h>
#include <mxml/dom/Pedal.h>
#include <mxml/dom/Wedge.h>


namespace mxml {

DirectionGeometryFactory::DirectionGeometryFactory(const Geometry* parentGeometry, const std::vector<MeasureGeometry*>& measureGeometries, const Metrics& metrics)
: _parentGeometry(parentGeometry),
  _measureGeometries(measureGeometries),
  _metrics(metrics)
{}

std::vector<std::unique_ptr<PlacementGeometry>> DirectionGeometryFactory::build() {
    _geometries.clear();

    for (auto& measure: _measureGeometries) {
        for (auto& node : measure->measure().nodes()) {
            if (const dom::Direction* direction = dynamic_cast<const dom::Direction*>(node.get()))
                buildDirection(*measure, *direction);
        }
    }

    return std::move(_geometries);
}

void DirectionGeometryFactory::buildDirection(const MeasureGeometry& measureGeom, const dom::Direction& direction) {
    if (dynamic_cast<const dom::Wedge*>(direction.type())) {
        buildWedge(measureGeom, direction);
        return;
    }

    if (dynamic_cast<const dom::Pedal*>(direction.type())) {
        buildPedal(measureGeom, direction);
        return;
    }

    if (dynamic_cast<const dom::OctaveShift*>(direction.type())) {
        buildOctaveShift(measureGeom, direction);
        return;
    }

    if (dynamic_cast<const dom::Coda*>(direction.type())) {
        buildCoda(measureGeom, direction);
        return;
    }

    if (dynamic_cast<const dom::Segno*>(direction.type())) {
        buildSegno(measureGeom, direction);
        return;
    }

    if (dynamic_cast<const dom::Words*>(direction.type()) ||
        dynamic_cast<const dom::Dynamics*>(direction.type()) ) {
        buildWords(measureGeom, direction);
        return;
    }
}

Point DirectionGeometryFactory::spanOffsetInParentGeometry(const MeasureGeometry& measureGeometry, Point p) {
    auto& spans = measureGeometry.spans();
    auto measureIndex = measureGeometry.measure().index();
    auto measureOrigin = spans.origin(measureIndex);

    p.x -= measureOrigin;
    p = _parentGeometry->convertFromGeometry(p, &measureGeometry);
    return p;
}

void DirectionGeometryFactory::placeDirection(PlacementGeometry& geometry) {
    auto location = geometry.location();

    if (geometry.placement() == dom::kPlacementAbove) {
        location.y = _metrics.staffOrigin(geometry.staff()) - Metrics::kStaffLineSpacing;
        geometry.setVerticalAnchorPointValues(1, 0);
    } else {
        location.y = _metrics.staffOrigin(geometry.staff()) + Metrics::staffHeight() + Metrics::kStaffLineSpacing;
        geometry.setVerticalAnchorPointValues(0, 0);
    }
    location.y -= _metrics.stavesHeight()/2;

    geometry.setLocation(location);
}

void DirectionGeometryFactory::buildWedge(const MeasureGeometry& measureGeom, const dom::Direction& direction) {
    using dom::Wedge;

    const Wedge& wedge = dynamic_cast<const Wedge&>(*direction.type());
    if (wedge.type() == Wedge::kStop) {
        auto it = std::find_if(_openSpanDirections.rbegin(), _openSpanDirections.rend(), [wedge](std::pair<const MeasureGeometry*, const dom::Direction*> pair) {
            if (const Wedge* startWedge = dynamic_cast<const Wedge*>(pair.second->type()))
                return startWedge->number() == wedge.number();
            return false;
        });

        if (it != _openSpanDirections.rend()) {
            buildWedge(*it->first, *it->second, measureGeom,direction);
            _openSpanDirections.erase(it.base() - 1);
        }
    } else if (wedge.type() != Wedge::kContinue) {
        _openSpanDirections.push_back(std::make_pair(&measureGeom, &direction));
    }
}

void DirectionGeometryFactory::buildWedge(const MeasureGeometry& startMeasureGeom, const dom::Direction& startDirection,
                                       const MeasureGeometry& stopMeasureGeom, const dom::Direction& stopDirection) {
    const Span& startSpan = *startMeasureGeom.spans().with(&startDirection);
    Point startLocation;
    startLocation.x = startSpan.start() + startSpan.eventOffset();
    startLocation = spanOffsetInParentGeometry(startMeasureGeom, startLocation);

    const Span& stopSpan = *stopMeasureGeom.spans().with(&stopDirection);
    Point stopLocation;
    stopLocation.x = stopSpan.start() + stopSpan.eventOffset();
    stopLocation = spanOffsetInParentGeometry(stopMeasureGeom, stopLocation);

    int staff = startDirection.staff();
    dom::Placement placement = startDirection.placement();
    if (!startDirection.placement().isPresent()) {
        if (staff == 1)
            placement = dom::kPlacementBelow;
        else
            placement = dom::kPlacementAbove;
    }

    if (placement == dom::kPlacementAbove) {
        startLocation.y = stopLocation.y = _metrics.staffOrigin(staff) - _metrics.staffDistance()/2;
    } else if (placement == dom::kPlacementBelow) {
        startLocation.y = stopLocation.y = _metrics.staffOrigin(staff) + Metrics::staffHeight() + _metrics.staffDistance()/2;
    }
    startLocation.y -= _metrics.stavesHeight()/2;
    stopLocation.y -= _metrics.stavesHeight()/2;

    std::unique_ptr<PlacementGeometry> geo(new SpanDirectionGeometry(startDirection, startLocation, stopDirection, stopLocation));
    geo->setPlacement(dom::Optional<dom::Placement>(placement, startDirection.placement().isPresent()));
    geo->setLocation(startLocation);

    _geometries.push_back(std::move(geo));
}

void DirectionGeometryFactory::buildPedal(const MeasureGeometry& measureGeom, const dom::Direction& direction) {
    using dom::Pedal;

    const Pedal& pedal = dynamic_cast<const Pedal&>(*direction.type());
    if (pedal.type() == dom::kStop) {
        auto it = std::find_if(_openSpanDirections.rbegin(), _openSpanDirections.rend(), [&direction](std::pair<const MeasureGeometry*, const dom::Direction*> pair) {
            if (dynamic_cast<const Pedal*>(pair.second->type()))
                return pair.second->staff() == direction.staff();
            return false;
        });

        if (it != _openSpanDirections.rend()) {
            buildPedal(*it->first, *it->second, measureGeom,direction);
            _openSpanDirections.erase(it.base() - 1);
        }
    } else if (pedal.type() != dom::kContinue) {
        _openSpanDirections.push_back(std::make_pair(&measureGeom, &direction));
    }
}

void DirectionGeometryFactory::buildPedal(const MeasureGeometry& startMeasureGeom, const dom::Direction& startDirection,
                                       const MeasureGeometry& stopMeasureGeom, const dom::Direction& stopDirection) {
    const Span& startSpan = *startMeasureGeom.spans().with(&startDirection);
    Point startLocation;
    startLocation.x = startSpan.start() + startSpan.eventOffset();
    startLocation = spanOffsetInParentGeometry(startMeasureGeom, startLocation);

    const Span& stopSpan = *stopMeasureGeom.spans().with(&stopDirection);
    Point stopLocation;
    stopLocation.x = stopSpan.start() + stopSpan.eventOffset();
    stopLocation = spanOffsetInParentGeometry(stopMeasureGeom, stopLocation);

    int staff = startDirection.staff();
    startLocation.y = stopLocation.y = _metrics.staffOrigin(staff) + Metrics::staffHeight() + _metrics.staffDistance()/2 - _metrics.stavesHeight()/2;

    std::unique_ptr<PlacementGeometry> geo(new PedalGeometry(startDirection, startLocation, stopDirection, stopLocation));
    geo->setLocation(startLocation);

    _geometries.push_back(std::move(geo));
}

void DirectionGeometryFactory::buildOctaveShift(const MeasureGeometry& measureGeom, const dom::Direction& direction) {
    using dom::OctaveShift;

    const OctaveShift& octaveShift = dynamic_cast<const OctaveShift&>(*direction.type());
    if (octaveShift.type == OctaveShift::kStop) {
        auto it = std::find_if(_openSpanDirections.rbegin(), _openSpanDirections.rend(), [&](std::pair<const MeasureGeometry*, const dom::Direction*> pair) {
            if (auto os = dynamic_cast<const OctaveShift*>(pair.second->type()))
                return pair.second->staff() == direction.staff() && os->number == octaveShift.number;
            return false;
        });

        if (it != _openSpanDirections.rend()) {
            buildOctaveShift(*it->first, *it->second, measureGeom,direction);
            _openSpanDirections.erase(it.base() - 1);
        }
    } else if (octaveShift.type != OctaveShift::kContinue) {
        _openSpanDirections.push_back(std::make_pair(&measureGeom, &direction));
    }
}

void DirectionGeometryFactory::buildOctaveShift(const MeasureGeometry& startMeasureGeom, const dom::Direction& startDirection,
                                             const MeasureGeometry& stopMeasureGeom, const dom::Direction& stopDirection) {
    const Span& startSpan = *startMeasureGeom.spans().with(&startDirection);
    Point startLocation;
    startLocation.x = startSpan.start() + startSpan.eventOffset();
    startLocation = spanOffsetInParentGeometry(startMeasureGeom, startLocation);

    const Span& stopSpan = *stopMeasureGeom.spans().with(&stopDirection);
    Point stopLocation;
    stopLocation.x = stopSpan.start() + stopSpan.eventOffset();
    stopLocation = spanOffsetInParentGeometry(stopMeasureGeom, stopLocation);

    startLocation.y = stopLocation.y = startMeasureGeom.origin().y - OctaveShiftGeometry::k8vaSize.height - 10;

    std::unique_ptr<PlacementGeometry> geo(new OctaveShiftGeometry(startDirection, startLocation, stopDirection, stopLocation));
    geo->setLocation(startLocation);

    _geometries.push_back(std::move(geo));
}

void DirectionGeometryFactory::buildWords(const MeasureGeometry& measureGeom, const dom::Direction& direction) {
    std::unique_ptr<WordsGeometry> wordsGeom(new WordsGeometry(direction));

    const Span& span = *measureGeom.spans().with(&direction);
    Point location;
    if (wordsGeom->size().width > 2*NoteGeometry::kQuarterWidth) {
        location.x = span.start() - span.leftMargin()/2;
    } else {
        location.x = span.start();
    }
    location = spanOffsetInParentGeometry(measureGeom, location);
    wordsGeom->setLocation(location);

    // Better placement defaults if the placement is not specified
    dom::Placement placement;
    if (!direction.placement().isPresent()) {
        if (wordsGeom->dynamics()) {
            if (direction.staff() == 1)
                placement = dom::kPlacementBelow;
            else
                placement = dom::kPlacementAbove;
        } else {
            if (direction.staff() == 1)
                placement = dom::kPlacementAbove;
            else
                placement = dom::kPlacementBelow;
        }
        wordsGeom->setPlacement(placement);
    }
    placeDirection(*wordsGeom);

    _geometries.push_back(std::move(wordsGeom));
}

void DirectionGeometryFactory::buildSegno(const MeasureGeometry& measureGeom, const dom::Direction& direction) {
    const dom::Segno& segno = dynamic_cast<const dom::Segno&>(*direction.type());
    std::unique_ptr<SegnoGeometry> segnoGeom(new SegnoGeometry(segno));
    
    // Segnos are always placed at the start of the measure
    Point location;
    location.x = measureGeom.location().x + SegnoGeometry::kSegnoSize.width;
    segnoGeom->setLocation(location);
    placeDirection(*segnoGeom);

    _geometries.push_back(std::move(segnoGeom));
}

void DirectionGeometryFactory::buildCoda(const MeasureGeometry& measureGeom, const dom::Direction& direction) {
    const dom::Coda& coda = dynamic_cast<const dom::Coda&>(*direction.type());
    std::unique_ptr<CodaGeometry> codaGeom(new CodaGeometry(coda));
    
    // Codas are always placed at the start of the measure
    Point location;
    location.x = measureGeom.location().x + CodaGeometry::kCodaSize.width;
    codaGeom->setLocation(location);
    placeDirection(*codaGeom);

    _geometries.push_back(std::move(codaGeom));
}

} // namespace mxml
