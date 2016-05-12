// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once
#include <mxml/geometry/Geometry.h>
#include <mxml/Metrics.h>

#include <memory>
#include <typeindex>
#include <set>
#include <unordered_map>
#include <vector>

namespace mxml {

class NoteGeometry;
class RestGeometry;

template <typename Comparator>
class CollisionResolver {
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
     Comparator used to sort collisions
     */
    class CollisionPairComparator : std::binary_function<const CollisionPair&, const CollisionPair&, bool> {
    public:
        bool operator()(const CollisionPair& g1, const CollisionPair& g2);

    private:
        Comparator _geometryComparator;
    };

    /**
     Comparator used to sort geometries by x position
     */
    class GeometryXComparator : std::binary_function<const Geometry*, const Geometry*, bool> {
    public:
        bool operator()(const Geometry* g1, const Geometry* g2);
    };

public:
    CollisionResolver(const Geometry& geometry, const Metrics& metrics);
    
    /**
     Resolve all collisions in the set of geometries.
     */
    void resolveCollisions();
    
protected:
    /**
     Add a vector of geometries to the set of geometries that are going be checked for collisions.
     */
    void addAllGeometries(const std::vector<std::unique_ptr<Geometry>>& geometries);
    
    /**
     Add a geometry to the set of geometries that are going be checked for collisions.
     
     @param geometry The geometry to add.
     */
    void addGeometry(Geometry* geometry);
    
    void addAllCollisions();
    bool addAllCollisions(Geometry* geometry);
    
    /**
     Check if two given geometries are colliding.
     */
    virtual bool colliding(const Geometry* g1, const Geometry* g2) const;

    /**
     Check if a given geometry is colliding.
     */
    virtual bool colliding(const Geometry* g1) const;
    
    /**
     Check if this collision pair can be resolved.
     
     @param pair The collision pair to check.
     */
    bool canResolveCollision(const CollisionPair& pair) const;

    /**
     Remove all pending collisions involving this geometry
     
     @param geometry The geometry to remove collisions for.
     */
    void removeCollisions(const Geometry *geometry);
    
    virtual void resolveCollision(const CollisionPair& pair) = 0;
    virtual bool isImmovable(const Geometry* geometry) const = 0;
    
protected:
    const Geometry& _geometry;
    const Metrics& _metrics;
    
    std::multiset<Geometry*, GeometryXComparator> _geometries;
    std::multiset<CollisionPair, CollisionPairComparator> _collisionPairs;
    Comparator _geometryTypeComparator;
};
    
}

#include "CollisionResolver.hh"
