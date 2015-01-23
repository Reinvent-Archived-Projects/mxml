//  Created by Steve Hosking on 2015-01-20.
//  Copyright (c) 2015 Venture Media Labs. All rights reserved.

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
