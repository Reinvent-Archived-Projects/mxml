//  Created by Alejandro Isaza on 2014-12-18.
//  Copyright (c) 2014 Venture Media Labs. All rights reserved.

#include "BarlineGeometry.h"
#include "ChordGeometry.h"
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

#include <typeindex>
#include <typeinfo>

namespace mxml {

    PartGeometryFactory::PartGeometryFactory(const dom::Part& part, const SpanCollection& spans) : _part(part), _spans(spans) {
    }

    std::unique_ptr<PartGeometry> PartGeometryFactory::build() {
        _partGeometry.reset(new PartGeometry(_part, _spans));

        coord_t offset = 0;
        for (auto& measure : _part.measures()) {
            std::unique_ptr<MeasureGeometry> geo(new MeasureGeometry(*measure, *_partGeometry, _spans));
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

        resolveCollisions();
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

        if (geometry.placement() == dom::PLACEMENT_ABOVE) {
            location.y = _partGeometry->staffOrigin(geometry.staff()) - PartGeometry::kStaffLineSpacing;
            geometry.setVerticalAnchorPointValues(1, 0);
        } else {
            location.y = _partGeometry->staffOrigin(geometry.staff()) + _partGeometry->staffHeight() + PartGeometry::kStaffLineSpacing;
            geometry.setVerticalAnchorPointValues(0, 0);
        }
        location.y -= _partGeometry->stavesHeight()/2;

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
        if (startDirection.placement() == dom::PLACEMENT_ABOVE && staff == 1) {
            startLocation.y = stopLocation.y = _partGeometry->staffOrigin(staff) - 3*PartGeometry::kStaffLineSpacing;
        } else if (startDirection.placement() == dom::PLACEMENT_ABOVE) {
            startLocation.y = stopLocation.y = _partGeometry->staffOrigin(staff) - _part.staffDistance()/2;
        } else if (startDirection.placement() == dom::PLACEMENT_BELOW) {
            startLocation.y = stopLocation.y = _partGeometry->staffOrigin(staff) + _partGeometry->staffHeight() + _part.staffDistance()/2;
        }
        startLocation.y -= _partGeometry->stavesHeight()/2;
        stopLocation.y -= _partGeometry->stavesHeight()/2;

        std::unique_ptr<PlacementGeometry> geo(new SpanDirectionGeometry(startDirection, startLocation, stopDirection, stopLocation));
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
        startLocation.y = stopLocation.y = _partGeometry->staffOrigin(staff) + _partGeometry->staffHeight() + _part.staffDistance()/2 - _partGeometry->stavesHeight()/2;

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
            if (!note->notations().isPresent())
                continue;

            const dom::Notations& notations = note->notations();
            for (auto& ornament : notations.ornaments())
                buildOrnament(measureGeom, chordGeom, ornament);
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
        if (!barline.ending().isPresent())
            return;
        const dom::Ending& ending = barline.ending();

        if ((ending.type() == dom::Ending::STOP || ending.type() == dom::Ending::DISCONTINUE) && _startEnding.isPresent()) {
            Point stopLocation;
            stopLocation.x = measureGeom.frame().max().x - 1;
            stopLocation.y = measureGeom.origin().y + MeasureGeometry::kVerticalPadding - EndingGeometry::kHeight - 10;

            std::unique_ptr<EndingGeometry> endingGeom(new EndingGeometry(_startEnding, _startEndingLocation, ending, stopLocation));
            endingGeom->setLocation(_startEndingLocation);
            _partGeometry->_directionGeometries.push_back(endingGeom.get());
            _partGeometry->addGeometry(std::move(endingGeom));

            _startEnding.reset();
        } else if (ending.type() == dom::Ending::START) {
            _startEnding.setPresentValue(ending);
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
                buildLyric(measureGeom, chordGeom, lyric);
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

    void PartGeometryFactory::resolveCollisions() {
        resolveDirectionDirectionCollisions();
    }

    void PartGeometryFactory::resolveDirectionDirectionCollisions() {
        // This is the collision priority of different direction types. Geometryies with lower numbers are less likely to
        // be moved.
        auto typeOrder = std::map<std::type_index, int>{};
        typeOrder[std::type_index(typeid(SpanDirectionGeometry*))] = 0;
        typeOrder[std::type_index(typeid(EndingGeometry*))] = 1;
        typeOrder[std::type_index(typeid(OrnamentsGeometry*))] = 2;
        typeOrder[std::type_index(typeid(LyricGeometry*))] = 3;
        typeOrder[std::type_index(typeid(DirectionGeometry*))] = 4;

        // Order geometries by type
        auto geoms = _partGeometry->_directionGeometries;
        std::sort(geoms.begin(), geoms.end(), [&](const PlacementGeometry* g1, const PlacementGeometry* g2) {
            auto& o1 = typeOrder[std::type_index(typeid(g1))];
            auto& o2 = typeOrder[std::type_index(typeid(g2))];
            if (o1 < o2)
                return true;
            if (o1 > o2)
                return false;

            // Order directions of the same type by size. This is because usually short directions are more closely related
            // to one particular note, and therefore should not be pushed out.
            return g1->size().width < g2->size().width;
        });

        const auto& size = geoms.size();
        std::vector<PlacementGeometry*> collisions;
        for (std::size_t i = 0; i < size; i += 1) {
            const auto& geom1 = geoms[i];
            collisions.clear();

            for (std::size_t j = i + 1; j < size; j += 1) {
                const auto& geom2 = geoms[j];

                if (intersect(geom1->frame(), geom2->frame())) {
                    if (collisions.empty())
                        collisions.push_back(geom1);
                    collisions.push_back(geom2);
                }
            }
            
            if (!collisions.empty())
                resolveDirectionCollision(collisions);
        }
    }
    
    void PartGeometryFactory::resolveDirectionCollision(std::vector<PlacementGeometry*>& geometries) {
        const auto& size = geometries.size();
        const auto& refFrame = geometries[0]->frame();
        const auto& placement = geometries[0]->placement().value();
        for (std::size_t i = 1; i < size; i += 1) {
            auto& geom = *geometries[i];
            
            // Don't move lyrics that collide with each other, they would loose continuity
            if (dynamic_cast<LyricGeometry*>(&geom) && dynamic_cast<LyricGeometry*>(geometries[0]))
                continue;
            
            // Try changing the placement
            if (dynamic_cast<DirectionGeometry*>(&geom) && !geom.placement().isPresent() && geometries[0]->placement().value() == geom.placement().value()) {
                swapPlacement(geom);
                
                // If it still collides, undo
                if (!_partGeometry->collidingGeometries(geom.frame()).empty()) {
                    swapPlacement(geom);
                } else {
                    continue;
                }
            }
            
            Rect frame = geom.frame();
            if (placement == dom::PLACEMENT_ABOVE)
                frame.origin.y = refFrame.origin.y - frame.size.height;
            else
                frame.origin.y = refFrame.origin.y + refFrame.size.height;
            geom.setFrame(frame);
        }
    }
    
    void PartGeometryFactory::swapPlacement(PlacementGeometry& geometry) {
        if (geometry.placement() == dom::PLACEMENT_ABOVE)
            geometry.setPlacement(absentOptional(dom::PLACEMENT_BELOW));
        else
            geometry.setPlacement(absentOptional(dom::PLACEMENT_ABOVE));
        
        placeDirection(geometry);
    }
}