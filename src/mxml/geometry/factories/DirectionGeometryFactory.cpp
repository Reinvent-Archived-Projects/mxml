// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "DirectionGeometryFactory.h"

#include <mxml/geometry/BarlineGeometry.h>
#include <mxml/geometry/CodaGeometry.h>
#include <mxml/geometry/EndingGeometry.h>
#include <mxml/geometry/NoteGeometry.h>
#include <mxml/geometry/MeasureGeometry.h>
#include <mxml/geometry/OctaveShiftGeometry.h>
#include <mxml/geometry/OrnamentsGeometry.h>
#include <mxml/geometry/PedalGeometry.h>
#include <mxml/geometry/SegnoGeometry.h>
#include <mxml/geometry/SpanDirectionGeometry.h>
#include <mxml/geometry/WordsGeometry.h>

#include <mxml/dom/OctaveShift.h>
#include <mxml/dom/Pedal.h>
#include <mxml/dom/Wedge.h>


namespace mxml {

DirectionGeometryFactory::DirectionGeometryFactory()
{}

DirectionGeometryFactory::DirectionGeometryFactory(const Geometry* parentGeometry, const std::vector<MeasureGeometry*>& measureGeometries, const Metrics& metrics)
: _parentGeometry(parentGeometry),
  _measureGeometries(measureGeometries),
  _metrics(&metrics)
{}

void DirectionGeometryFactory::reset(const Geometry* parentGeometry, const std::vector<MeasureGeometry*>& measureGeometries, const Metrics& metrics) {
    _metrics = &metrics;
    _parentGeometry = parentGeometry;
    _measureGeometries = measureGeometries;
    _geometries.clear();

    for (auto& pair : _openSpanDirections) {
        _previouslyOpenSpanDirections.push_back(pair.second);
    }
    _openSpanDirections.clear();
}

std::vector<std::unique_ptr<PlacementGeometry>> DirectionGeometryFactory::build() {
    _geometries.clear();

    for (auto& measure: _measureGeometries) {
        for (auto& node : measure->measure().nodes()) {
            if (const dom::Direction* direction = dynamic_cast<const dom::Direction*>(node.get()))
                buildDirection(*measure, *direction);
        }
    }

    // Finish any direction that started but did not stop
    for (auto& pair : _openSpanDirections) {
        auto measureGeometry = pair.first;
        auto direction = pair.second;
        if (dynamic_cast<const dom::OctaveShift*>(direction->type())) {
            buildOctaveShiftToEdge(*measureGeometry, *direction);
        } else if (dynamic_cast<const dom::Pedal*>(direction->type())) {
            buildPedalToEdge(*measureGeometry, *direction);
        }
    }

    // Build directions that neither started or stopped
    for (auto& direction : _previouslyOpenSpanDirections) {
        if (dynamic_cast<const dom::OctaveShift*>(direction->type())) {
            buildOctaveShiftFromEdgeToEdge(*direction);
        } else if (dynamic_cast<const dom::Pedal*>(direction->type())) {
            buildPedalFromEdgeToEdge(*direction);
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

    if (geometry.placement() == dom::Placement::Above) {
        location.y = _metrics->staffOrigin(geometry.staff()) - Metrics::kStaffLineSpacing;
        geometry.setVerticalAnchorPointValues(1, 0);
    } else {
        location.y = _metrics->staffOrigin(geometry.staff()) + Metrics::staffHeight() + Metrics::kStaffLineSpacing;
        geometry.setVerticalAnchorPointValues(0, 0);
    }

    geometry.setLocation(location);
}

void DirectionGeometryFactory::buildWedge(const MeasureGeometry& measureGeom, const dom::Direction& direction) {
    using dom::Wedge;

    const Wedge& wedge = dynamic_cast<const Wedge&>(*direction.type());
    if (wedge.type() == Wedge::Type::Stop) {
        auto it = std::find_if(_openSpanDirections.rbegin(), _openSpanDirections.rend(), [wedge](std::pair<const MeasureGeometry*, const dom::Direction*> pair) {
            if (const Wedge* startWedge = dynamic_cast<const Wedge*>(pair.second->type()))
                return startWedge->number() == wedge.number();
            return false;
        });

        if (it != _openSpanDirections.rend()) {
            buildWedge(*it->first, *it->second, measureGeom,direction);
            _openSpanDirections.erase(it.base() - 1);
        }
    } else if (wedge.type() != Wedge::Type::Continue) {
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
            placement = dom::Placement::Below;
        else
            placement = dom::Placement::Above;
    }

    if (placement == dom::Placement::Above) {
        startLocation.y = stopLocation.y = _metrics->staffOrigin(staff) - _metrics->staffDistance()/2;
    } else if (placement == dom::Placement::Below) {
        startLocation.y = stopLocation.y = _metrics->staffOrigin(staff) + Metrics::staffHeight() + _metrics->staffDistance()/2;
    }

    std::unique_ptr<PlacementGeometry> geo(new SpanDirectionGeometry(&startDirection, startLocation, &stopDirection, stopLocation));
    geo->setPlacement(dom::Optional<dom::Placement>(placement, startDirection.placement().isPresent()));
    geo->setLocation(startLocation);

    _geometries.push_back(std::move(geo));
}

void DirectionGeometryFactory::buildPedal(const MeasureGeometry& measureGeom, const dom::Direction& direction) {
    using dom::Pedal;

    const Pedal& pedal = dynamic_cast<const Pedal&>(*direction.type());
    if (pedal.type() == dom::kStop) {
        auto pair = pullPedalStart(measureGeom, direction);
        if (pair.first) {
            buildPedal(*pair.first, *pair.second, measureGeom, direction);
        } else {
            buildPedalFromEdge(*pair.second, measureGeom, direction);
        }
    } else if (pedal.type() != dom::kContinue) {
        _openSpanDirections.push_back(std::make_pair(&measureGeom, &direction));
    }
}

DirectionGeometryFactory::MDPair DirectionGeometryFactory::pullPedalStart(const MeasureGeometry& stopMeasure, const dom::Direction& stopDirection) {
    auto it = std::find_if(_openSpanDirections.rbegin(), _openSpanDirections.rend(), [&](std::pair<const MeasureGeometry*, const dom::Direction*> pair) {
        if (dynamic_cast<const dom::Pedal*>(pair.second->type()))
            return pair.second->staff() == stopDirection.staff();
        return false;
    });
    if (it != _openSpanDirections.rend()) {
        auto pair = *it;
        _openSpanDirections.erase(it.base() - 1);
        return pair;
    }

    auto pit = std::find_if(_previouslyOpenSpanDirections.rbegin(), _previouslyOpenSpanDirections.rend(), [&](const dom::Direction* d) {
        if (dynamic_cast<const dom::Pedal*>(d->type()))
            return d->staff() == stopDirection.staff();
        return false;
    });
    if (pit != _previouslyOpenSpanDirections.rend()) {
        auto pair = MDPair{nullptr, *pit};
        _previouslyOpenSpanDirections.erase(pit.base() - 1);
        return pair;
    }
    
    return {};
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
    startLocation.y = stopLocation.y = _metrics->staffOrigin(staff) + Metrics::staffHeight() + _metrics->staffDistance()/2;

    std::unique_ptr<PlacementGeometry> geo(new PedalGeometry(&startDirection, startLocation, &stopDirection, stopLocation));
    geo->setLocation(startLocation);

    _geometries.push_back(std::move(geo));
}

void DirectionGeometryFactory::buildPedalFromEdge(const dom::Direction& startDirection, const MeasureGeometry& stopMeasureGeom, const dom::Direction& stopDirection) {
    Point startLocation;
    startLocation.x = _parentGeometry->bounds().min().x;

    const Span& stopSpan = *stopMeasureGeom.spans().with(&stopDirection);
    Point stopLocation;
    stopLocation.x = stopSpan.start() + stopSpan.eventOffset();
    stopLocation = spanOffsetInParentGeometry(stopMeasureGeom, stopLocation);

    int staff = stopDirection.staff();
    startLocation.y = stopLocation.y = _metrics->staffOrigin(staff) + Metrics::staffHeight() + _metrics->staffDistance()/2;

    std::unique_ptr<PedalGeometry> geo(new PedalGeometry(&startDirection, startLocation, &stopDirection, stopLocation));
    geo->setContinuation();
    geo->setLocation(startLocation);

    _geometries.push_back(std::move(geo));
}

void DirectionGeometryFactory::buildPedalToEdge(const MeasureGeometry& startMeasureGeom, const dom::Direction& startDirection) {
    const Span& startSpan = *startMeasureGeom.spans().with(&startDirection);
    Point startLocation;
    startLocation.x = startSpan.start() + startSpan.eventOffset();
    startLocation = spanOffsetInParentGeometry(startMeasureGeom, startLocation);

    Point stopLocation;
    stopLocation.x = _parentGeometry->bounds().max().x;

    int staff = startDirection.staff();
    startLocation.y = stopLocation.y = _metrics->staffOrigin(staff) + Metrics::staffHeight() + _metrics->staffDistance()/2;

    std::unique_ptr<PlacementGeometry> geo(new PedalGeometry(&startDirection, startLocation, nullptr, stopLocation));
    geo->setLocation(startLocation);

    _geometries.push_back(std::move(geo));
}

void DirectionGeometryFactory::buildPedalFromEdgeToEdge(const dom::Direction& startDirection) {
    Point startLocation;
    startLocation.x = _parentGeometry->bounds().min().x;

    Point stopLocation;
    stopLocation.x = _parentGeometry->bounds().max().x;

    int staff = startDirection.staff();
    startLocation.y = stopLocation.y = _metrics->staffOrigin(staff) + Metrics::staffHeight() + _metrics->staffDistance()/2;

    std::unique_ptr<PedalGeometry> geo(new PedalGeometry(&startDirection, startLocation, nullptr, stopLocation));
    geo->setContinuation();
    geo->setLocation(startLocation);
    
    _geometries.push_back(std::move(geo));
}

void DirectionGeometryFactory::buildOctaveShift(const MeasureGeometry& measureGeom, const dom::Direction& direction) {
    const dom::OctaveShift& octaveShift = dynamic_cast<const dom::OctaveShift&>(*direction.type());
    if (octaveShift.type == dom::OctaveShift::Type::Stop) {
        const MeasureGeometry* startMeasure;
        const dom::Direction* startDirection;
        std::tie(startMeasure, startDirection) = pullOctaveShiftStart(measureGeom, direction);

        if (startMeasure) {
            buildOctaveShift(*startMeasure, *startDirection, measureGeom, direction);
        } else {
            buildOctaveShiftFromEdge(*startDirection, measureGeom, direction);
        }
    } else if (octaveShift.type != dom::OctaveShift::Type::Continue) {
        _openSpanDirections.push_back(std::make_pair(&measureGeom, &direction));
    }
}

DirectionGeometryFactory::MDPair DirectionGeometryFactory::pullOctaveShiftStart(const MeasureGeometry& stopMeasure, const dom::Direction& stopDirection) {
    const dom::OctaveShift& octaveShift = dynamic_cast<const dom::OctaveShift&>(*stopDirection.type());

    auto it = std::find_if(_openSpanDirections.rbegin(), _openSpanDirections.rend(), [&](std::pair<const MeasureGeometry*, const dom::Direction*> pair) {
        if (auto os = dynamic_cast<const dom::OctaveShift*>(pair.second->type()))
            return pair.second->staff() == stopDirection.staff() && os->number == octaveShift.number;
        return false;
    });
    if (it != _openSpanDirections.rend()) {
        auto pair = *it;
        _openSpanDirections.erase(it.base() - 1);
        return pair;
    }

    auto pit = std::find_if(_previouslyOpenSpanDirections.rbegin(), _previouslyOpenSpanDirections.rend(), [&](const dom::Direction* d) {
        if (auto os = dynamic_cast<const dom::OctaveShift*>(d->type()))
            return d->staff() == stopDirection.staff() && os->number == octaveShift.number;
        return false;
    });
    if (pit != _previouslyOpenSpanDirections.rend()) {
        auto pair = MDPair{nullptr, *pit};
        _previouslyOpenSpanDirections.erase(pit.base() - 1);
        return pair;
    }

    return {};
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

    std::unique_ptr<PlacementGeometry> geo(new OctaveShiftGeometry(&startDirection, startLocation, &stopDirection, stopLocation));
    geo->setLocation(startLocation);

    _geometries.push_back(std::move(geo));
}

void DirectionGeometryFactory::buildOctaveShiftFromEdge(const dom::Direction& startDirection, const MeasureGeometry& stopMeasureGeom, const dom::Direction& stopDirection) {
    Point startLocation;
    startLocation.x = _parentGeometry->bounds().min().x;

    const Span& stopSpan = *stopMeasureGeom.spans().with(&stopDirection);
    Point stopLocation;
    stopLocation.x = stopSpan.start() + stopSpan.eventOffset();
    stopLocation = spanOffsetInParentGeometry(stopMeasureGeom, stopLocation);

    startLocation.y = stopLocation.y = stopMeasureGeom.origin().y - OctaveShiftGeometry::k8vaSize.height - 10;

    std::unique_ptr<OctaveShiftGeometry> geo(new OctaveShiftGeometry(&startDirection, startLocation, &stopDirection, stopLocation));
    geo->setContinuation();
    geo->setLocation(startLocation);

    _geometries.push_back(std::move(geo));
}

void DirectionGeometryFactory::buildOctaveShiftToEdge(const MeasureGeometry& startMeasureGeom, const dom::Direction& startDirection) {
    const Span& startSpan = *startMeasureGeom.spans().with(&startDirection);
    Point startLocation;
    startLocation.x = startSpan.start() + startSpan.eventOffset();
    startLocation = spanOffsetInParentGeometry(startMeasureGeom, startLocation);

    Point stopLocation;
    stopLocation.x = _parentGeometry->bounds().max().x;

    startLocation.y = stopLocation.y = startMeasureGeom.origin().y - OctaveShiftGeometry::k8vaSize.height - 10;

    std::unique_ptr<PlacementGeometry> geo(new OctaveShiftGeometry(&startDirection, startLocation, nullptr, stopLocation));
    geo->setLocation(startLocation);

    _geometries.push_back(std::move(geo));
}

void DirectionGeometryFactory::buildOctaveShiftFromEdgeToEdge(const dom::Direction& startDirection) {
    Point startLocation;
    startLocation.x = _parentGeometry->bounds().min().x;

    Point stopLocation;
    stopLocation.x = _parentGeometry->bounds().max().x;

    startLocation.y = stopLocation.y = - OctaveShiftGeometry::k8vaSize.height - 10;

    std::unique_ptr<OctaveShiftGeometry> geo(new OctaveShiftGeometry(&startDirection, startLocation, nullptr, stopLocation));
    geo->setContinuation();
    geo->setLocation(startLocation);
    
    _geometries.push_back(std::move(geo));
}

void DirectionGeometryFactory::buildWords(const MeasureGeometry& measureGeom, const dom::Direction& direction) {
    std::unique_ptr<WordsGeometry> wordsGeom(new WordsGeometry(direction));

    const Span& span = *measureGeom.spans().with(&direction);
    Point location;
    if (dynamic_cast<dom::Dynamics*>(direction.type()))
        location.x = span.start() + span.eventOffset();
    else
        location.x = span.start() - span.leftMargin()/2;
    wordsGeom->setLocation(spanOffsetInParentGeometry(measureGeom, location));

    // Don't place outside parent's bounds
    if (wordsGeom->frame().max().x  > _parentGeometry->bounds().max().x) {
        location.x = _parentGeometry->bounds().max().x - wordsGeom->size().width + wordsGeom->anchorPoint().x;
        wordsGeom->setLocation(location);
    } else if (wordsGeom->frame().min().x < _parentGeometry->bounds().min().x) {
        location.x = _parentGeometry->bounds().min().x + wordsGeom->anchorPoint().x;
        wordsGeom->setLocation(location);
    }

    // Better placement defaults if the placement is not specified
    dom::Placement placement;
    if (!direction.placement().isPresent()) {
        if (wordsGeom->dynamics()) {
            if (direction.staff() == 1)
                placement = dom::Placement::Below;
            else
                placement = dom::Placement::Above;
        } else {
            if (direction.staff() == 1)
                placement = dom::Placement::Above;
            else
                placement = dom::Placement::Below;
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
