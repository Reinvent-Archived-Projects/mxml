//  Created by Steve Hosking on 2015-01-20.
//  Copyright (c) 2015 Venture Media Labs. All rights reserved.

#pragma once
#include "CollisionResolver.h"

namespace mxml {
    
    class NoteGeometry;
    class RestGeometry;
    
    /**
     Comparator used to sort geometries by type
     */
    class VerticalTypeComparator : std::binary_function<const Geometry*, const Geometry*, bool> {
    public:
        VerticalTypeComparator();
        bool operator()(const Geometry* g1, const Geometry* g2);
        std::map<std::type_index, int> typeOrder;
    };
    
    class VerticalResolver : public CollisionResolver<VerticalTypeComparator> {
    public:
        VerticalResolver(const Geometry& geometry, const Metrics& metrics);
        
    protected:
        void resolveCollision(const CollisionPair& pair);
        void resolveCollision(const Geometry* g1, Geometry* g2);
        void resolveCollision(const Geometry* g1, RestGeometry* rest);
        
        bool isImmovable(const Geometry* geometry) const;
        
        void readdGeometry(Geometry* geometry);

        Rect chooseBestFrame(Geometry* geometry, const Rect& f1, const Rect& f2);
        
        std::map<Geometry*, int> _collisionCount;
    };
    
}
