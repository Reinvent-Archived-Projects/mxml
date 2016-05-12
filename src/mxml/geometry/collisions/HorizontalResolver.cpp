// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "HorizontalResolver.h"

#include <mxml/geometry/ChordGeometry.h>
#include <mxml/geometry/NoteGeometry.h>

#include <utility>

namespace mxml {
    
    HorizontalTypeComparator::HorizontalTypeComparator() {
        // This is the collision priority of different geometry types. Geometries with lower numbers are less likely to
        // be moved.
        typeOrder[std::type_index(typeid(NoteGeometry))] = 0;
    }
    
    bool HorizontalTypeComparator::HorizontalTypeComparator::operator()(const Geometry* g1, const Geometry* g2) {
        auto& o1 = typeOrder[std::type_index(typeid(*g1))];
        auto& o2 = typeOrder[std::type_index(typeid(*g2))];
        if (o1 < o2)
            return true;
        if (o1 > o2)
            return false;
        
        if (g1->center().x < g2->center().x)
            return true;
        if (g1->center().x > g2->center().x)
            return false;
        
        return g1->center().y < g2->center().y;
    }
    
    HorizontalResolver::HorizontalResolver(const Geometry& geometry, const Metrics& metrics) : CollisionResolver(geometry, metrics) {
    }
    
    void HorizontalResolver::resolveCollision(const CollisionPair& pair) {
        if (!canResolveCollision(pair))
            return;
        
        // Put the less movable geometry first, so that we move the other one
        Geometry* first = pair.firstGeometry();
        Geometry* second = pair.secondGeometry();
        
        if (!isImmovable(pair.firstGeometry()) && isImmovable(pair.secondGeometry()))
            std::swap(first, second);
        else if (!_geometryTypeComparator(pair.firstGeometry(), pair.secondGeometry()))
            std::swap(first, second);
        
        auto note1Geometry = dynamic_cast<NoteGeometry*>(first);
        auto note2Geometry = dynamic_cast<NoteGeometry*>(second);
        if (note1Geometry && note2Geometry)
            resolveCollision(note1Geometry, note2Geometry);
    }
    
    void HorizontalResolver::resolveCollision(NoteGeometry* ng1, NoteGeometry* ng2) {
        // Ignore notes that belong to the same Chord
        if (ng1->parentGeometry() == ng2->parentGeometry())
            return;

        auto& n1 = ng1->note();
        auto& n2 = ng2->note();

        // Prefer to move 'larger' notes
        if (n1.type() > n2.type())
            std::swap(ng1, ng2);

        // Ignore notes with stems on different voices that match exactly
        if (n1.voice() != n2.voice() &&
            n1.type() < dom::Note::Type::Half && n2.type() < dom::Note::Type::Half &&
            n1.pitch && n2.pitch && n1.pitch->step() == n2.pitch->step())
            return;
        
        // We don't handle moving notes with ties yet
        if (ng2->tieGeometry())
            return;
        
        // When resoliving a note collision we need to move the parent geometry
        ChordGeometry *chordGeometry = (ChordGeometry *)ng2->parentGeometry();
        Rect f1 = _geometry.convertFromGeometry(ng1->frame(), ng1->parentGeometry());
        Rect f2 = _geometry.convertFromGeometry(chordGeometry->frame(), chordGeometry->parentGeometry());
        f2.origin.x = f1.origin.x + f1.size.width;
        chordGeometry->setFrame(chordGeometry->parentGeometry()->convertFromRoot(f2));
        
        removeCollisions(ng2);
    }
    
    bool HorizontalResolver::colliding(const Geometry* g1, const Geometry* g2) const {
        Rect f1 = _geometry.convertFromGeometry(g1->frame(), g1->parentGeometry());
        Rect f2 = _geometry.convertFromGeometry(g2->frame(), g2->parentGeometry());
        return intersect(f1, f2, 0, std::min(f1.size.height/2, f2.size.height/2));
    }
    
    bool HorizontalResolver::isImmovable(const Geometry* geometry) const {
        return typeid(*geometry) != typeid(NoteGeometry);
    }

}