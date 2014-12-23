//  Created by Alejandro Isaza on 2014-12-22.
//  Copyright (c) 2014 Venture Media Labs. All rights reserved.

#pragma once
#include "PartGeometry.h"

#include <memory>
#include <typeindex>
#include <set>
#include <unordered_map>
#include <vector>

namespace mxml {
    class CollisionHandler {
    protected:
        /**
         Collision pair.
         */
        class CollisionPair {
        public:
            CollisionPair(Geometry* first, Geometry* second) : _firstGeometry(first), _secondGeometry(second) {}

            Geometry* firstGeometry() const {
                return _firstGeometry;
            }
            Geometry* secondGeometry() const {
                return _secondGeometry;
            }

            bool operator==(const CollisionPair& rhs) const {
                return _firstGeometry == rhs._firstGeometry && _secondGeometry == rhs._secondGeometry;
            }

        private:
            Geometry* _firstGeometry;
            Geometry* _secondGeometry;
        };

        /**
         Comparator used to sort geometries by type
         */
        class GeometryTypeComparator : std::binary_function<const Geometry*, const Geometry*, bool> {
        public:
            GeometryTypeComparator();
            bool operator()(const Geometry* g1, const Geometry* g2);
            std::map<std::type_index, int> typeOrder;
        };

        /**
         Comparator used to sort collisions
         */
        class CollisionPairComparator : std::binary_function<const CollisionPair&, const CollisionPair&, bool> {
        public:
            bool operator()(const CollisionPair& g1, const CollisionPair& g2);

        private:
            GeometryTypeComparator _geometryComparator;
        };

        /**
         Comparator used to sort geometries by x position
         */
        class GeometryXComparator : std::binary_function<const Geometry*, const Geometry*, bool> {
        public:
            bool operator()(const Geometry* g1, const Geometry* g2);
        };

    public:
        CollisionHandler(const PartGeometry& partGeometry);

        /**
         Add a vector of geometries to the set of geometries that are going be checked for collisions.
         */
        void addAllGeometries(const std::vector<std::unique_ptr<Geometry>>& geometries);

        /**
         Add a geometry to the set of geometries that are going be checked for collisions.

         @param geometry The geometry to add.
         */
        void addGeometry(Geometry* geometry);

        /**
         Resolve all collisions in the set of geometries.
         */
        void resolveCollisions();

    protected:
        void addAllCollisions();
        bool addAllCollisions(Geometry* geometry);

        void resolveCollision(CollisionPair& pair);
        void resolveCollision(const Geometry* g1, Geometry* g2);

        bool colliding(const Geometry* g1, const Geometry* g2) const;
        static bool isImmovable(const Geometry* geometry);

    private:
        const PartGeometry& _partGeometry;
        GeometryTypeComparator _geometryTypeComparator;

        std::multiset<Geometry*, GeometryXComparator> _geometries;
        std::multiset<CollisionPair, CollisionPairComparator> _collisionPairs;
    };
}
