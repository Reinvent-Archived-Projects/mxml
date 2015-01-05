//  Created by Alejandro Isaza on 2014-12-18.
//  Copyright (c) 2014 Venture Media Labs. All rights reserved.

#include "BarlineGeometry.h"
#include "ChordGeometry.h"
#include "CollisionHandler.h"
#include "DirectionGeometry.h"
#include "EndingGeometry.h"
#include "MeasureGeometry.h"
#include "LyricGeometry.h"
#include "OrnamentsGeometry.h"
#include "PartGeometryFactory.h"
#include "PedalGeometry.h"
#include "SpanDirectionGeometry.h"
#include "TieGeometryFactory.h"

#include <mxml/dom/Pedal.h>
#include <mxml/dom/Wedge.h>

#include <mxml/Metrics.h>

#include <typeindex>
#include <typeinfo>

namespace mxml {

    PartGeometryFactory::PartGeometryFactory(const dom::Part& part, const SpanCollection& spans) : _part(part), _spans(spans) {
    }

    std::unique_ptr<PartGeometry> PartGeometryFactory::build() {
        _partGeometry.reset(new PartGeometry(_part, _spans));

        coord_t offset = 0;
        for (auto& measure : _part.measures()) {
            std::unique_ptr<MeasureGeometry> geo(new MeasureGeometry(*measure, *_partGeometry, _spans, _attributesManager));
            geo->setHorizontalAnchorPointValues(0, 0);
            geo->setLocation({offset, 0});
            offset += geo->size().width;

            _partGeometry->_measureGeometries.push_back(geo.get());
            _partGeometry->addGeometry(std::move(geo));
        }

        buildDirections();
        buildOrnaments();
        buildEndings();
        buildLyrics();

        TieGeometryFactory factory(*_partGeometry);
        auto ties = factory.buildTieGeometries(_partGeometry->geometries());
        for (auto& tie : ties) {
            _partGeometry->_tieGeometries.push_back(tie.get());
            _partGeometry->addGeometry(std::move(tie));
        }
        
        CollisionHandler collisionHandler(*_partGeometry);
        collisionHandler.resolveCollisions();
        
        _partGeometry->setBounds(_partGeometry->subGeometriesFrame());

        return std::move(_partGeometry);
    }

    void PartGeometryFactory::buildDirections() {
        for (auto measure: _partGeometry->_measureGeometries) {
            for (auto& node : measure->measure().nodes()) {
                if (const dom::Direction* direction = dynamic_cast<const dom::Direction*>(node.get()))
                    buildDirection(*measure, *direction);
            }
        }
    }

    void PartGeometryFactory::buildDirection(const MeasureGeometry& measureGeom, const dom::Direction& direction) {
        if (dynamic_cast<const dom::Wedge*>(direction.type())) {
            buildWedge(measureGeom, direction);
            return;
        }

        if (dynamic_cast<const dom::Pedal*>(direction.type())) {
            buildPedal(measureGeom, direction);
            return;
        }

        std::unique_ptr<DirectionGeometry> dirGeom(new DirectionGeometry(direction));

        Point location;
        const Span& span = *measureGeom.spans().with(&direction);
        if (dirGeom->size().width > 2*NoteGeometry::kQuarterWidth) {
            location.x = span.start() - span.leftMargin()/2;
        } else {
            location.x = span.start();
        }
        dirGeom->setLocation(location);

        // Better placement defaults if the placement is not specified
        dom::Placement placement = direction.placement();
        if (!direction.placement().isPresent()) {
            if (dynamic_cast<const dom::Dynamics*>(direction.type())) {
                if (direction.staff() == 1)
                    placement = dom::PLACEMENT_BELOW;
                else
                    placement = dom::PLACEMENT_ABOVE;
            } else if (dynamic_cast<const dom::Words*>(direction.type())) {
                if (direction.staff() == 1)
                    placement = dom::PLACEMENT_ABOVE;
                else
                    placement = dom::PLACEMENT_BELOW;
            }
            dirGeom->setPlacement(placement);
        }
        placeDirection(*dirGeom);

        _partGeometry->_directionGeometries.push_back(dirGeom.get());
        _partGeometry->addGeometry(std::move(dirGeom));
    }

    void PartGeometryFactory::placeDirection(PlacementGeometry& geometry) {
        auto location = geometry.location();
        auto& part = _partGeometry->part();
        
        if (geometry.placement() == dom::PLACEMENT_ABOVE) {
            location.y = Metrics::staffOrigin(part, geometry.staff()) - Metrics::kStaffLineSpacing;
            geometry.setVerticalAnchorPointValues(1, 0);
        } else {
            location.y = Metrics::staffOrigin(part, geometry.staff()) + Metrics::staffHeight() + Metrics::kStaffLineSpacing;
            geometry.setVerticalAnchorPointValues(0, 0);
        }
        location.y -= Metrics::stavesHeight(part)/2;

        geometry.setLocation(location);
    }

    void PartGeometryFactory::buildWedge(const MeasureGeometry& measureGeom, const dom::Direction& direction) {
        using dom::Wedge;

        const Wedge& wedge = dynamic_cast<const Wedge&>(*direction.type());
        if (wedge.type() == Wedge::TYPE_STOP) {
            auto it = std::find_if(_openSpanDirections.rbegin(), _openSpanDirections.rend(), [wedge](std::pair<const MeasureGeometry*, const dom::Direction*> pair) {
                if (const Wedge* startWedge = dynamic_cast<const Wedge*>(pair.second->type()))
                    return startWedge->number() == wedge.number();
                return false;
            });

            if (it != _openSpanDirections.rend()) {
                buildWedge(*it->first, *it->second, measureGeom,direction);
                _openSpanDirections.erase(it.base() - 1);
            }
        } else if (wedge.type() != Wedge::TYPE_CONTINUE) {
            _openSpanDirections.push_back(std::make_pair(&measureGeom, &direction));
        }
    }

    void PartGeometryFactory::buildWedge(const MeasureGeometry& startMeasureGeom, const dom::Direction& startDirection,
                                  const MeasureGeometry& stopMeasureGeom, const dom::Direction& stopDirection) {
        const Span& startSpan = *startMeasureGeom.spans().with(&startDirection);
        Point startLocation;
        startLocation.x = startSpan.start() + startSpan.eventOffset();

        const Span& stopSpan = *stopMeasureGeom.spans().with(&stopDirection);
        Point stopLocation;
        stopLocation.x = stopSpan.start() + stopSpan.eventOffset();

        int staff = startDirection.staff();
        dom::Placement placement = startDirection.placement();
        if (!startDirection.placement().isPresent()) {
            if (staff == 1)
                placement = dom::PLACEMENT_BELOW;
            else
                placement = dom::PLACEMENT_ABOVE;
        }
        
        auto& part = _partGeometry->part();
        
        if (placement == dom::PLACEMENT_ABOVE) {
            startLocation.y = stopLocation.y = Metrics::staffOrigin(part, staff) - _part.staffDistance()/2;
        } else if (placement == dom::PLACEMENT_BELOW) {
            startLocation.y = stopLocation.y = Metrics::staffOrigin(part, staff) + Metrics::staffHeight() + _part.staffDistance()/2;
        }
        startLocation.y -= Metrics::stavesHeight(part)/2;
        stopLocation.y -= Metrics::stavesHeight(part)/2;

        std::unique_ptr<PlacementGeometry> geo(new SpanDirectionGeometry(startDirection, startLocation, stopDirection, stopLocation));
        geo->setPlacement(dom::Optional<dom::Placement>(placement, startDirection.placement().isPresent()));
        geo->setLocation(startLocation);

        _partGeometry->_directionGeometries.push_back(geo.get());
        _partGeometry->addGeometry(std::move(geo));
    }

    void PartGeometryFactory::buildPedal(const MeasureGeometry& measureGeom, const dom::Direction& direction) {
        using dom::Pedal;

        const Pedal& pedal = dynamic_cast<const Pedal&>(*direction.type());
        if (pedal.type() == dom::TYPE_STOP) {
            auto it = std::find_if(_openSpanDirections.rbegin(), _openSpanDirections.rend(), [&direction](std::pair<const MeasureGeometry*, const dom::Direction*> pair) {
                if (dynamic_cast<const Pedal*>(pair.second->type()))
                    return pair.second->staff() == direction.staff();
                return false;
            });

            if (it != _openSpanDirections.rend()) {
                buildPedal(*it->first, *it->second, measureGeom,direction);
                _openSpanDirections.erase(it.base() - 1);
            }
        } else if (pedal.type() != dom::TYPE_CONTINUE) {
            _openSpanDirections.push_back(std::make_pair(&measureGeom, &direction));
        }
    }

    void PartGeometryFactory::buildPedal(const MeasureGeometry& startMeasureGeom, const dom::Direction& startDirection,
                                  const MeasureGeometry& stopMeasureGeom, const dom::Direction& stopDirection) {
        const Span& startSpan = *startMeasureGeom.spans().with(&startDirection);
        Point startLocation;
        startLocation.x = startSpan.start() + startSpan.eventOffset();

        const Span& stopSpan = *stopMeasureGeom.spans().with(&stopDirection);
        Point stopLocation;
        stopLocation.x = stopSpan.start() + stopSpan.eventOffset();

        int staff = startDirection.staff();
        startLocation.y = stopLocation.y = Metrics::staffOrigin(_part, staff) + Metrics::staffHeight() + _part.staffDistance()/2 - Metrics::stavesHeight(_part)/2;

        std::unique_ptr<PlacementGeometry> geo(new PedalGeometry(startDirection, startLocation, stopDirection, stopLocation));
        geo->setLocation(startLocation);

        _partGeometry->_directionGeometries.push_back(geo.get());
        _partGeometry->addGeometry(std::move(geo));
    }

    void PartGeometryFactory::buildOrnaments() {
        for (auto measure: _partGeometry->_measureGeometries) {
            for (auto& geom : measure->geometries()) {
                if (const ChordGeometry* chord = dynamic_cast<const ChordGeometry*>(geom.get()))
                    buildOrnaments(*measure, *chord);
            }
        }
    }

    void PartGeometryFactory::buildOrnaments(const MeasureGeometry& measureGeom, const ChordGeometry& chordGeom) {
        for (auto& note : chordGeom.chord().notes()) {
            if (!note->notations())
                continue;

            auto& notations = note->notations();
            for (auto& ornament : notations->ornaments())
                buildOrnament(measureGeom, chordGeom, *ornament);
        }
    }

    void PartGeometryFactory::buildOrnament(const MeasureGeometry& measureGeom, const ChordGeometry& chordGeom, const dom::Ornaments& ornaments) {
        const dom::EmptyPlacement* placement = ornaments.placement();
        if (!placement)
            return;

        const int staff = chordGeom.chord().firstNote()->staff();
        std::unique_ptr<PlacementGeometry> geo(new OrnamentsGeometry(ornaments, staff));

        const Span& span = *measureGeom.spans().with(&chordGeom.chord());
        Point location;
        location.x = span.start() + span.eventOffset();
        geo->setLocation(location);

        placeDirection(*geo);

        // Avoid placing on top of existing geometries
        Rect frame = _partGeometry->convertToGeometry(geo->frame(), &measureGeom);
        auto collidingGeom = measureGeom.collidingGeometry(frame);
        if (collidingGeom) {
            if (placement->placement() == dom::PLACEMENT_ABOVE) {
                Point origin = _partGeometry->convertFromGeometry(collidingGeom->origin(), &measureGeom);
                location.y = origin.y - geo->size().height/2 - 4;
                geo->setLocation(location);
            } else {
                Point point = _partGeometry->convertFromGeometry(collidingGeom->frame().max(), &measureGeom);
                location.y = point.y + geo->size().height/2 + 4;
                geo->setLocation(location);
            }
        }

        _partGeometry->_directionGeometries.push_back(geo.get());
        _partGeometry->addGeometry(std::move(geo));
    }

    void PartGeometryFactory::buildEndings() {
        for (auto measure: _partGeometry->_measureGeometries) {
            for (auto& geom : measure->geometries()) {
                if (const BarlineGeometry* barline = dynamic_cast<const BarlineGeometry*>(geom.get()))
                    buildEndings(*measure, *barline);
            }
        }
    }

    void PartGeometryFactory::buildEndings(const MeasureGeometry& measureGeom, const BarlineGeometry& barlineGeom) {
        const dom::Barline& barline = barlineGeom.barline();
        if (!barline.ending())
            return;
        
        auto& ending = barline.ending();

        if ((ending->type() == dom::Ending::STOP || ending->type() == dom::Ending::DISCONTINUE) && _startEnding.isPresent()) {
            Point stopLocation;
            stopLocation.x = measureGeom.frame().max().x - 1;
            stopLocation.y = measureGeom.origin().y + MeasureGeometry::kVerticalPadding - EndingGeometry::kHeight - 10;

            std::unique_ptr<EndingGeometry> endingGeom(new EndingGeometry(_startEnding, _startEndingLocation, *ending, stopLocation));
            endingGeom->setLocation(_startEndingLocation);
            _partGeometry->_directionGeometries.push_back(endingGeom.get());
            _partGeometry->addGeometry(std::move(endingGeom));

            _startEnding.reset();
        } else if (ending->type() == dom::Ending::START) {
            _startEnding.setPresentValue(*ending);
            _startEndingLocation = measureGeom.location();
            _startEndingLocation.y = measureGeom.origin().y + MeasureGeometry::kVerticalPadding - EndingGeometry::kHeight - 10;
        }
    }

    void PartGeometryFactory::buildLyrics() {
        for (auto measure: _partGeometry->_measureGeometries) {
            for (auto& geom : measure->geometries()) {
                if (const ChordGeometry* chord = dynamic_cast<const ChordGeometry*>(geom.get()))
                    buildLyrics(*measure, *chord);
            }
        }
    }

    void PartGeometryFactory::buildLyrics(const MeasureGeometry& measureGeom, const ChordGeometry& chordGeom) {
        for (auto noteGeom : chordGeom.notes()) {
            for (auto& lyric : noteGeom->note().lyrics()) {
                buildLyric(measureGeom, chordGeom, *lyric);
            }
        }
    }

    void PartGeometryFactory::buildLyric(const MeasureGeometry& measureGeom, const ChordGeometry& chordGeom, const dom::Lyric& lyric) {
        const int staff = chordGeom.chord().firstNote()->staff();
        std::unique_ptr<PlacementGeometry> geo(new LyricGeometry(lyric, staff));

        const Span& span = *measureGeom.spans().with(&chordGeom.chord());
        Point location;
        location.x = span.start() + span.eventOffset();
        geo->setLocation(location);

        placeDirection(*geo);

        location = geo->location();
        if (lyric.number() > 1) {
            if (geo->placement() == dom::PLACEMENT_ABOVE)
                location.y -= (lyric.number() - 1) * geo->size().height;
            else
                location.y += (lyric.number() - 1) * geo->size().height;
            geo->setLocation(location);
        }

        _partGeometry->_directionGeometries.push_back(geo.get());
        _partGeometry->addGeometry(std::move(geo));
    }
}