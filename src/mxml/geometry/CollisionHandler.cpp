//  Created by Alejandro Isaza on 2014-12-22.
//  Copyright (c) 2014 Venture Media Labs. All rights reserved.

#include "CollisionHandler.h"

#include "AccidentalGeometry.h"
#include "ArticulationGeometry.h"
#include "BeamGeometry.h"
#include "ClefGeometry.h"
#include "WordsGeometry.h"
#include "DotGeometry.h"
#include "EndingGeometry.h"
#include "FermataGeometry.h"
#include "KeyGeometry.h"
#include "LyricGeometry.h"
#include "NoteGeometry.h"
#include "OrnamentsGeometry.h"
#include "PedalGeometry.h"
#include "RestGeometry.h"
#include "SpanDirectionGeometry.h"
#include "StemGeometry.h"
#include "TieGeometry.h"
#include "TimeSignatureGeometry.h"
#include <mxml/Metrics.h>

#include <typeinfo>

static const int kMaxCollisionsPerGeometry = 10;


namespace mxml {

    CollisionHandler::CollisionHandler(const PartGeometry& partGeometry) : _partGeometry(partGeometry) {
        addAllGeometries(partGeometry.geometries());
    }

    void CollisionHandler::addAllGeometries(const std::vector<std::unique_ptr<Geometry>>& geometries) {
        using std::type_index;
        const auto measureGeometryIndex = type_index(typeid(MeasureGeometry));
        const auto chordGeometryIndex = type_index(typeid(ChordGeometry));

        for (auto& geometry : geometries) {
            auto index = type_index(typeid(*geometry));
            if (index == measureGeometryIndex || index == chordGeometryIndex) {
                addAllGeometries(geometry->geometries());
                continue;
            }

            auto it = _geometryTypeComparator.typeOrder.find(index);
            if (it != _geometryTypeComparator.typeOrder.end())
                addGeometry(geometry.get());
        }
    }

    void CollisionHandler::addGeometry(Geometry* geometry) {
        _geometries.insert(geometry);
    }

    void CollisionHandler::resolveCollisions() {
        addAllCollisions();

        while (!_collisionPairs.empty()) {
            auto it = _collisionPairs.begin();
            auto pair = *it;
            _collisionPairs.erase(it);
            resolveCollision(pair);
        }
    }

    void CollisionHandler::addAllCollisions() {
        for (auto i = _geometries.begin(); i != _geometries.end(); ++i) {
            Rect f1 = _partGeometry.convertFromGeometry((*i)->frame(), (*i)->parentGeometry());

            for (auto j = std::next(i); j != _geometries.end(); ++j) {
                Rect f2 = _partGeometry.convertFromGeometry((*j)->frame(), (*j)->parentGeometry());
                if (f2.origin.x > f1.origin.x + f1.size.width)
                    break;

                if (colliding(*i, *j))
                    _collisionPairs.emplace(*i, *j);
            }
        }
    }

    bool CollisionHandler::addAllCollisions(Geometry* geometry) {
        Rect f1 = _partGeometry.convertFromGeometry(geometry->frame(), geometry->parentGeometry());
        bool foundCollision = false;

        for (auto i = _geometries.begin(); i != _geometries.end(); ++i) {
            if (*i == geometry)
                continue;

            Rect f2 = _partGeometry.convertFromGeometry((*i)->frame(), (*i)->parentGeometry());
            if (f2.origin.x > f1.origin.x + f1.size.width)
                break;

            if (colliding(geometry, *i)) {
                foundCollision = true;
                _collisionPairs.emplace(geometry, *i);
            }
        }

        return foundCollision;
    }

    void CollisionHandler::resolveCollision(CollisionPair& pair) {
        // Ignore geometries with no extension
        if (pair.firstGeometry()->size().width == 0 || pair.firstGeometry()->size().height == 0)
            return;
        if (pair.secondGeometry()->size().width == 0 || pair.secondGeometry()->size().height == 0)
            return;

        /// Ignore pairs of immovable geometries
        if (isImmovable(pair.firstGeometry()) && isImmovable(pair.secondGeometry()))
            return;

        // Put the less movable geometry first, so that we move the other one
        Geometry* first = pair.firstGeometry();
        Geometry* second = pair.secondGeometry();

        if (!isImmovable(pair.firstGeometry()) && isImmovable(pair.secondGeometry()))
            std::swap(first, second);
        else if (!_geometryTypeComparator(pair.firstGeometry(), pair.secondGeometry()))
            std::swap(first, second);

        auto noteGeometry = dynamic_cast<NoteGeometry*>(first);
        auto restGeometry = dynamic_cast<RestGeometry*>(second);
        if (noteGeometry && restGeometry)
            resolveCollision(noteGeometry, restGeometry);
        resolveCollision(first, second);
    }

    bool CollisionHandler::isImmovable(const Geometry* geometry) {
        return
            typeid(*geometry) == typeid(ClefGeometry) ||
            typeid(*geometry) == typeid(KeyGeometry) ||
            typeid(*geometry) == typeid(TimeSignatureGeometry) ||
            typeid(*geometry) == typeid(NoteGeometry) ||
            typeid(*geometry) == typeid(DotGeometry) ||
            typeid(*geometry) == typeid(AccidentalGeometry) ||
            typeid(*geometry) == typeid(StemGeometry) ||
            typeid(*geometry) == typeid(BeamGeometry) ||
            typeid(*geometry) == typeid(TieGeometry) ||
            typeid(*geometry) == typeid(EndingGeometry) ||
            typeid(*geometry) == typeid(LyricGeometry);
    }

    void CollisionHandler::resolveCollision(const Geometry* g1, Geometry* g2) {
        Rect f1 = _partGeometry.convertFromGeometry(g1->frame(), g1->parentGeometry());
        Rect f2 = _partGeometry.convertFromGeometry(g2->frame(), g2->parentGeometry());

        // Only move things that are already outside the staves further away
        auto& part = _partGeometry.part();
        coord_t top = Metrics::staffOrigin(part, 1) - Metrics::stavesHeight(part)/2;
        coord_t bottom = Metrics::staffOrigin(part, part.staves()) - Metrics::stavesHeight(part)/2;
        if (f2.origin.y <= top)
            f2.origin.y = f1.origin.y - f2.size.height - 1;
        else if (f2.max().y >= bottom)
            f2.origin.y = f1.origin.y + f1.size.height + 1;
        else
            return;

        g2->setFrame(g2->parentGeometry()->convertFromRoot(f2));
        readdGeometry(g2);
    }

    void CollisionHandler::resolveCollision(const NoteGeometry* note, RestGeometry* rest) {
        Rect noteFrame = _partGeometry.convertFromGeometry(note->frame(), note->parentGeometry());
        Rect restFrame = _partGeometry.convertFromGeometry(rest->frame(), rest->parentGeometry());

        // Only move things that are already outside the staves further away
        if (note->note().stem() == dom::STEM_UP)
            restFrame.origin.y = noteFrame.origin.y + noteFrame.size.height + 1;
        else if (note->note().stem() == dom::STEM_DOWN)
            restFrame.origin.y = noteFrame.origin.y - restFrame.size.height - 1;

        rest->setFrame(rest->parentGeometry()->convertFromRoot(restFrame));
        readdGeometry(rest);
    }

    void CollisionHandler::readdGeometry(Geometry* geometry) {
        _collisionCount[geometry] += 1;
        if (_collisionCount[geometry] > kMaxCollisionsPerGeometry)
            return;

        // Remove all pending collisions involving rest
        auto it = _collisionPairs.begin();
        while (it != _collisionPairs.end()) {
            auto next = std::next(it);

            if (it->firstGeometry() == geometry || it->secondGeometry() == geometry)
                _collisionPairs.erase(it);

            it = next;
        }

        // Re-add collisions involving rest
        addAllCollisions(geometry);
    }

    bool CollisionHandler::colliding(const Geometry* g1, const Geometry* g2) const {
        Rect f1 = _partGeometry.convertFromGeometry(g1->frame(), g1->parentGeometry());
        Rect f2 = _partGeometry.convertFromGeometry(g2->frame(), g2->parentGeometry());
        return intersect(f1, f2);
    }

    CollisionHandler::GeometryTypeComparator::GeometryTypeComparator() {
        // This is the collision priority of different geometry types. Geometries with lower numbers are less likely to
        // be moved.
        typeOrder[std::type_index(typeid(ClefGeometry))] = 0;
        typeOrder[std::type_index(typeid(KeyGeometry))] = 0;
        typeOrder[std::type_index(typeid(TimeSignatureGeometry))] = 0;
        typeOrder[std::type_index(typeid(NoteGeometry))] = 1;
        typeOrder[std::type_index(typeid(DotGeometry))] = 1;
        typeOrder[std::type_index(typeid(AccidentalGeometry))] = 1;
        typeOrder[std::type_index(typeid(StemGeometry))] = 1;
        typeOrder[std::type_index(typeid(BeamGeometry))] = 1;
        typeOrder[std::type_index(typeid(TieGeometry))] = 1;
        typeOrder[std::type_index(typeid(EndingGeometry))] = 1;
        typeOrder[std::type_index(typeid(LyricGeometry))] = 2;
        typeOrder[std::type_index(typeid(ArticulationGeometry))] = 3;
        typeOrder[std::type_index(typeid(FermataGeometry))] = 3;
        typeOrder[std::type_index(typeid(RestGeometry))] = 4;
        typeOrder[std::type_index(typeid(SpanDirectionGeometry))] = 5;
        typeOrder[std::type_index(typeid(OrnamentsGeometry))] = 6;
        typeOrder[std::type_index(typeid(PedalGeometry))] = 7;
        typeOrder[std::type_index(typeid(WordsGeometry))] = 8;
    }

    bool CollisionHandler::GeometryTypeComparator::operator()(const Geometry* g1, const Geometry* g2) {
        auto& o1 = typeOrder[std::type_index(typeid(*g1))];
        auto& o2 = typeOrder[std::type_index(typeid(*g2))];
        if (o1 < o2)
            return true;
        if (o1 > o2)
            return false;

        // Order geometries of the same type by size. This is because usually short geometries are more closely related
        // to one particular note, and therefore should not be pushed out.
        return g1->size().width < g2->size().width;
    }

    bool CollisionHandler::CollisionPairComparator::operator()(const CollisionPair& p1, const CollisionPair& p2) {
        auto g11 = _geometryComparator(p1.firstGeometry(), p1.secondGeometry()) ? p1.firstGeometry() : p1.secondGeometry();
        auto g21 = _geometryComparator(p2.firstGeometry(), p2.secondGeometry()) ? p2.firstGeometry() : p2.secondGeometry();
        if (_geometryComparator(g11, g21))
            return true;
        if (_geometryComparator(g21, g11))
            return false;

        auto g12 = _geometryComparator(p1.firstGeometry(), p1.secondGeometry()) ? p1.secondGeometry() : p1.firstGeometry();
        auto g22 = _geometryComparator(p2.firstGeometry(), p2.secondGeometry()) ? p2.secondGeometry() : p2.firstGeometry();
        return _geometryComparator(g12, g22);
    }

    bool CollisionHandler::GeometryXComparator::operator()(const Geometry* g1, const Geometry* g2) {
        Rect f1 = g1->parentGeometry()->convertToRoot(g1->frame());
        Rect f2 = g2->parentGeometry()->convertToRoot(g2->frame());
        return f1.origin.x < f2.origin.x;
    }
}
