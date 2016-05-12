// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <mxml/geometry/ChordGeometry.h>
#include <mxml/geometry/MeasureGeometry.h>

#include <typeinfo>

namespace mxml {

template <typename Comparator>
CollisionResolver<Comparator>::CollisionResolver(const Geometry& geometry, const Metrics& metrics)
: _geometry(geometry), _metrics(metrics) {
    addAllGeometries(_geometry.geometries());
}

template <typename Comparator>
void CollisionResolver<Comparator>::addAllGeometries(const std::vector<std::unique_ptr<Geometry>>& geometries) {
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

template <typename Comparator>
void CollisionResolver<Comparator>::addGeometry(Geometry* geometry) {
    _geometries.insert(geometry);
}

template <typename Comparator>
void CollisionResolver<Comparator>::addAllCollisions() {
    for (auto i = _geometries.begin(); i != _geometries.end(); ++i) {
        Rect f1 = _geometry.convertFromGeometry((*i)->frame(), (*i)->parentGeometry());
        
        for (auto j = std::next(i); j != _geometries.end(); ++j) {
            Rect f2 = _geometry.convertFromGeometry((*j)->frame(), (*j)->parentGeometry());
            if (f2.origin.x > f1.origin.x + f1.size.width)
                break;
            
            if (colliding(*i, *j))
                _collisionPairs.emplace(*i, *j);
        }
    }
}

template <typename Comparator>
bool CollisionResolver<Comparator>::addAllCollisions(Geometry* geometry) {
    Rect f1 = _geometry.convertFromGeometry(geometry->frame(), geometry->parentGeometry());
    bool foundCollision = false;
    
    for (auto i = _geometries.begin(); i != _geometries.end(); ++i) {
        if (*i == geometry)
            continue;
        
        Rect f2 = _geometry.convertFromGeometry((*i)->frame(), (*i)->parentGeometry());
        if (f2.origin.x > f1.origin.x + f1.size.width)
            break;
        
        if (colliding(geometry, *i)) {
            foundCollision = true;
            _collisionPairs.emplace(geometry, *i);
        }
    }
    
    return foundCollision;
}

template <typename Comparator>
bool CollisionResolver<Comparator>::colliding(const Geometry* g1, const Geometry* g2) const {
    Rect f1 = _geometry.convertFromGeometry(g1->frame(), g1->parentGeometry());
    Rect f2 = _geometry.convertFromGeometry(g2->frame(), g2->parentGeometry());
    return intersect(f1, f2);
}

template <typename Comparator>
bool CollisionResolver<Comparator>::colliding(const Geometry* geometry) const {
    Rect frame = _geometry.convertFromGeometry(geometry->frame(), geometry->parentGeometry());

    for (auto i = _geometries.begin(); i != _geometries.end(); ++i) {
        if (*i == geometry)
            continue;

        Rect f2 = _geometry.convertFromGeometry((*i)->frame(), (*i)->parentGeometry());
        if (f2.origin.x > frame.origin.x + frame.size.width)
            break;

        if (colliding(geometry, *i))
            return true;
    }
    
    return false;
}

template <typename Comparator>
void CollisionResolver<Comparator>::resolveCollisions() {
    addAllCollisions();
    
    while (!_collisionPairs.empty()) {
        auto it = _collisionPairs.begin();
        auto pair = *it;
        _collisionPairs.erase(it);
        resolveCollision(pair);
    }
}

template <typename Comparator>
bool CollisionResolver<Comparator>::canResolveCollision(const CollisionPair& pair) const {
    // Ignore geometries with no extension
    if (pair.firstGeometry()->size().width == 0 || pair.firstGeometry()->size().height == 0)
        return false;
    if (pair.secondGeometry()->size().width == 0 || pair.secondGeometry()->size().height == 0)
        return false;
    
    /// Ignore pairs of immovable geometries
    if (isImmovable(pair.firstGeometry()) && isImmovable(pair.secondGeometry()))
        return false;
    
    return true;
}

template <typename Comparator>
void CollisionResolver<Comparator>::removeCollisions(const Geometry *geometry) {
    auto it = _collisionPairs.begin();
    while (it != _collisionPairs.end()) {
        auto next = std::next(it);
        
        if (it->firstGeometry() == geometry || it->secondGeometry() == geometry)
            _collisionPairs.erase(it);
        
        it = next;
    }
}

template <typename Comparator>
bool CollisionResolver<Comparator>::CollisionPairComparator::operator()(const CollisionPair& p1, const CollisionPair& p2) {
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

template <typename Comparator>
bool CollisionResolver<Comparator>::GeometryXComparator::operator()(const Geometry* g1, const Geometry* g2) {
    Rect f1 = g1->parentGeometry()->convertToRoot(g1->frame());
    Rect f2 = g2->parentGeometry()->convertToRoot(g2->frame());
    return f1.origin.x < f2.origin.x;
}

}
