//  Created by Steve Hosking on 2015-01-20.
//  Copyright (c) 2015 Venture Media Labs. All rights reserved.

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
    
    HorizontalResolver::HorizontalResolver(const PartGeometry& partGeometry) : CollisionResolver(partGeometry) {
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
    
    void HorizontalResolver::resolveCollision(NoteGeometry* n1, NoteGeometry* n2) {
        // Ignore notes that belong to the same Chord
        if (n1->parentGeometry() == n2->parentGeometry())
            return;
        
        // Prefer to move 'larger' notes
        if (n1->note().type() > n2->note().type())
            std::swap(n1, n2);
        
        // We don't handle moving notes with ties yet
        if (n2->tieGeometry())
            return;
        
        // When resoliving a note collision we need to move the parent geometry
        ChordGeometry *chordGeometry = (ChordGeometry *)n2->parentGeometry();
        Rect f1 = _partGeometry.convertFromGeometry(n1->frame(), n1->parentGeometry());
        Rect f2 = _partGeometry.convertFromGeometry(chordGeometry->frame(), chordGeometry->parentGeometry());
        f2.origin.x = f1.origin.x + f1.size.width;
        chordGeometry->setFrame(chordGeometry->parentGeometry()->convertFromRoot(f2));
        
        removeCollisions(n2);
    }
    
    bool HorizontalResolver::colliding(const Geometry* g1, const Geometry* g2) const {
        Rect f1 = _partGeometry.convertFromGeometry(g1->frame(), g1->parentGeometry());
        Rect f2 = _partGeometry.convertFromGeometry(g2->frame(), g2->parentGeometry());
        return intersect(f1, f2, 0, std::min(f1.size.height/2, f2.size.height/2));
    }
    
    bool HorizontalResolver::isImmovable(const Geometry* geometry) const {
        return typeid(*geometry) != typeid(NoteGeometry);
    }

}