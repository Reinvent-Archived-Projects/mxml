// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once
#include "CollisionResolver.h"

namespace mxml {
    
    class NoteGeometry;
    
    /**
     Comparator used to sort geometries by type
     */
    class HorizontalTypeComparator : std::binary_function<const Geometry*, const Geometry*, bool> {
    public:
        HorizontalTypeComparator();
        bool operator()(const Geometry* g1, const Geometry* g2);
        std::map<std::type_index, int> typeOrder;
    };
    
    class HorizontalResolver : public CollisionResolver<HorizontalTypeComparator> {
    public:
        HorizontalResolver(const Geometry& geometry, const Metrics& metrics);
        
    protected:
        void resolveCollision(const CollisionPair& pair);
        void resolveCollision(NoteGeometry* n1, NoteGeometry* n2);
        
        bool colliding(const Geometry* g1, const Geometry* g2) const;
        
        bool isImmovable(const Geometry* geometry) const;
    };
    
}
