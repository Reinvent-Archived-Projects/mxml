// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "VerticalResolver.h"

#include <mxml/geometry/AccidentalGeometry.h>
#include <mxml/geometry/ArticulationGeometry.h>
#include <mxml/geometry/BeamGeometry.h>
#include <mxml/geometry/CodaGeometry.h>
#include <mxml/geometry/ClefGeometry.h>
#include <mxml/geometry/SegnoGeometry.h>
#include <mxml/geometry/DotGeometry.h>
#include <mxml/geometry/EndingGeometry.h>
#include <mxml/geometry/FermataGeometry.h>
#include <mxml/geometry/KeyGeometry.h>
#include <mxml/geometry/LyricGeometry.h>
#include <mxml/geometry/NoteGeometry.h>
#include <mxml/geometry/OctaveShiftGeometry.h>
#include <mxml/geometry/OrnamentsGeometry.h>
#include <mxml/geometry/PedalGeometry.h>
#include <mxml/geometry/RestGeometry.h>
#include <mxml/geometry/SpanDirectionGeometry.h>
#include <mxml/geometry/StemGeometry.h>
#include <mxml/geometry/TieGeometry.h>
#include <mxml/geometry/TimeSignatureGeometry.h>
#include <mxml/geometry/TupletGeometry.h>
#include <mxml/geometry/WordsGeometry.h>

#include <mxml/Metrics.h>

#include <typeinfo>

static const int kMaxCollisionsPerGeometry = 10;

namespace mxml {
    
    VerticalTypeComparator::VerticalTypeComparator() {
        // This is the collision priority of different geometry types. Geometries with lower numbers are less likely to
        // be moved.
        typeOrder[std::type_index(typeid(ClefGeometry))] = 0;
        typeOrder[std::type_index(typeid(KeyGeometry))] = 0;
        typeOrder[std::type_index(typeid(TimeSignatureGeometry))] = 0;
        typeOrder[std::type_index(typeid(NoteGeometry))] = 1;
        typeOrder[std::type_index(typeid(DotGeometry))] = 1;
        typeOrder[std::type_index(typeid(AccidentalGeometry))] = 1;
        typeOrder[std::type_index(typeid(StemGeometry))] = 1;
        typeOrder[std::type_index(typeid(EndingGeometry))] = 1;
        typeOrder[std::type_index(typeid(LyricGeometry))] = 2;
        typeOrder[std::type_index(typeid(ArticulationGeometry))] = 3;
        typeOrder[std::type_index(typeid(FermataGeometry))] = 3;
        typeOrder[std::type_index(typeid(RestGeometry))] = 4;
        typeOrder[std::type_index(typeid(SpanDirectionGeometry))] = 5;
        typeOrder[std::type_index(typeid(OrnamentsGeometry))] = 6;
        typeOrder[std::type_index(typeid(TupletGeometry))] = 7;
        typeOrder[std::type_index(typeid(OctaveShiftGeometry))] = 7;
        typeOrder[std::type_index(typeid(PedalGeometry))] = 7;
        typeOrder[std::type_index(typeid(SegnoGeometry))] = 8;
        typeOrder[std::type_index(typeid(CodaGeometry))] = 8;
        typeOrder[std::type_index(typeid(WordsGeometry))] = 9;
    }
    
    bool VerticalTypeComparator::operator()(const Geometry* g1, const Geometry* g2) {
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
    
    VerticalResolver::VerticalResolver(const Geometry& geometry, const Metrics& metrics) : CollisionResolver(geometry, metrics) {
    }
    
    void VerticalResolver::resolveCollision(const CollisionPair& pair) {
        if (!canResolveCollision(pair))
            return;
        
        // Put the less movable geometry first, so that we move the other one
        Geometry* first = pair.firstGeometry();
        Geometry* second = pair.secondGeometry();
        
        if (!isImmovable(pair.firstGeometry()) && isImmovable(pair.secondGeometry()))
            std::swap(first, second);
        else if (!_geometryTypeComparator(pair.firstGeometry(), pair.secondGeometry()))
            std::swap(first, second);
        
        auto noteGeometry = dynamic_cast<NoteGeometry*>(first);
        auto stemGeometry = dynamic_cast<StemGeometry*>(first);
        auto restGeometry = dynamic_cast<RestGeometry*>(second);

        if (noteGeometry && restGeometry)
            resolveCollision(noteGeometry, restGeometry);
        else if (stemGeometry && restGeometry)
            resolveCollision(stemGeometry, restGeometry);
        else if (auto placementGeometry = dynamic_cast<TupletGeometry*>(second))
            resolveCollision(first, placementGeometry);
        else if (auto placementGeometry = dynamic_cast<OctaveShiftGeometry*>(second))
            resolveCollision(first, placementGeometry);
        else if (auto placementGeometry = dynamic_cast<PedalGeometry*>(second))
            resolveCollision(first, placementGeometry);
        else
            resolveCollision(first, second);
    }
    
    void VerticalResolver::resolveCollision(const Geometry* g1, Geometry* g2) {
        Rect f1 = _geometry.convertFromGeometry(g1->frame(), g1->parentGeometry());
        Rect f2 = _geometry.convertFromGeometry(g2->frame(), g2->parentGeometry());
        
        // Only move things that are already outside the staves further away
        coord_t top = _metrics.staffOrigin(1);
        coord_t bottom = _metrics.staffOrigin(static_cast<int>(_metrics.staves()));
        if (f2.origin.y <= top)
            f2.origin.y = f1.origin.y - f2.size.height - 1;
        else if (f2.max().y >= bottom)
            f2.origin.y = f1.origin.y + f1.size.height + 1;
        else
            return;
        
        g2->setFrame(g2->parentGeometry()->convertFromRoot(f2));
        
        removeCollisions(g2);
        readdGeometry(g2);
    }

    void VerticalResolver::resolveCollision(const Geometry* g1, RestGeometry* rest) {
        Rect stemFrame = _geometry.convertFromGeometry(g1->frame(), g1->parentGeometry());
        Rect restFrame = _geometry.convertFromGeometry(rest->frame(), rest->parentGeometry());
        auto upFrame = restFrame;
        auto downFrame = restFrame;

        // Reduce the frame size to avoid close call collisions
        upFrame.size.height -= 2;
        downFrame.size.height -= 2;

        // Try moving it both up and down
        while (intersect(stemFrame, upFrame)) upFrame.origin.y -= 10;
        while (intersect(stemFrame, downFrame)) downFrame.origin.y += 10;

        // Reset frame size
        upFrame.size = restFrame.size;
        downFrame.size = restFrame.size;

        auto bestFrame = chooseBestFrame(rest, upFrame, downFrame);
        rest->setFrame(rest->parentGeometry()->convertFromRoot(bestFrame));

        removeCollisions(rest);
        readdGeometry(rest);
    }

    void VerticalResolver::resolveCollision(const Geometry* g1, PlacementGeometry* placement) {
        Rect frame = _geometry.convertFromGeometry(g1->frame(), g1->parentGeometry());
        Rect placementFrame = _geometry.convertFromGeometry(placement->frame(), placement->parentGeometry());

        if (placement->placement() == dom::Placement::Above && placement->staff() == 1)
            placementFrame.origin.y = frame.origin.y - placementFrame.size.height - 1;
        else if (placement->placement() == dom::Placement::Below && placement->staff() == _metrics.staves())
            placementFrame.origin.y = frame.origin.y + placementFrame.size.height + 1;

        placement->setFrame(placement->parentGeometry()->convertFromRoot(placementFrame));

        removeCollisions(placement);
        readdGeometry(placement);
    }

    Rect VerticalResolver::chooseBestFrame(Geometry* geometry, const Rect& f1, const Rect& f2) {
        Rect currentFrame = _geometry.convertFromGeometry(geometry->frame(), geometry->parentGeometry());

        geometry->setFrame(geometry->parentGeometry()->convertFromRoot(f1));
        auto firstColliding = colliding(geometry);

        geometry->setFrame(geometry->parentGeometry()->convertFromRoot(f2));
        auto secondColliding = colliding(geometry);

        if (firstColliding && secondColliding)
            return currentFrame;

        if (firstColliding)
            return f2;

        if (secondColliding)
            return f1;

        if (std::abs(f1.origin.y - currentFrame.origin.y) < std::abs(f2.origin.y - currentFrame.origin.y))
            return f1;
        return f2;
    }

    bool VerticalResolver::isImmovable(const Geometry* geometry) const {
        return (typeid(*geometry) == typeid(ClefGeometry) ||
                typeid(*geometry) == typeid(KeyGeometry) ||
                typeid(*geometry) == typeid(TimeSignatureGeometry) ||
                typeid(*geometry) == typeid(NoteGeometry) ||
                typeid(*geometry) == typeid(DotGeometry) ||
                typeid(*geometry) == typeid(AccidentalGeometry) ||
                typeid(*geometry) == typeid(StemGeometry) ||
                typeid(*geometry) == typeid(BeamGeometry) ||
                typeid(*geometry) == typeid(TieGeometry) ||
                typeid(*geometry) == typeid(EndingGeometry) ||
                typeid(*geometry) == typeid(LyricGeometry));
    }
    
    void VerticalResolver::readdGeometry(Geometry* geometry) {
        _collisionCount[geometry] += 1;
        if (_collisionCount[geometry] > kMaxCollisionsPerGeometry)
            return;
        
        // Re-add collisions involving rest
        addAllCollisions(geometry);
    }
    
}
