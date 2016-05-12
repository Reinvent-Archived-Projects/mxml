// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once
#include <mxml/geometry/Rect.h>
#include <mxml/dom/Node.h>

#include <functional>
#include <memory>
#include <vector>
#include <typeindex>

namespace mxml {

class Geometry {
public:
    Geometry();
    virtual ~Geometry() = default;

    /**
     Set the horizontal anchor point values. The anchor point is caculated by `x = multiplier * width + constant`.

     @param multiplier The width multiplier
     @param constant   The constant
     */
    void setHorizontalAnchorPointValues(coord_t multiplier, coord_t constant) {
        _horizontalAnchorPointMultiplier = multiplier;
        _horizontalAnchorPointConstant = constant;
    }

    /**
     Set the vertical anchor point values. The anchor point is caculated by `y = multiplier * height + constant`.

     @param multiplier The height multiplier
     @param constant   The constant
     */
    void setVerticalAnchorPointValues(coord_t multiplier, coord_t constant) {
        _verticalAnchorPointMultiplier = multiplier;
        _verticalAnchorPointConstant = constant;
    }

    /**
     The anchor point of the geometry. Should be a value between `0` and `width` for the `x` component and between `0`
     and `height` for the `y` component.
     */
    const Point anchorPoint() const {
        return {
            _horizontalAnchorPointMultiplier * _size.width + _horizontalAnchorPointConstant,
            _verticalAnchorPointMultiplier * _size.height + _verticalAnchorPointConstant
        };
    }

    /**
     The anchor point of the geometry for a given size. Should be a value between `0` and `size.width` for the `x`
     component and between `0` and `size.height` for the `y` component.
     */
    const Point anchorPoint(const Size& size) const {
        return {
            _horizontalAnchorPointMultiplier * size.width + _horizontalAnchorPointConstant,
            _verticalAnchorPointMultiplier * size.height + _verticalAnchorPointConstant
        };
    }

    /**
      The location of the geometry's anchor point in the parent geometry's coordinate system.
     */
    const Point& location() const {
        return _location;
    }
    void setLocation(const Point& location) {
        _location = location;
    }

    /**
     The size of the geometry.
     */
    const Size& size() const {
        return _size;
    }
    void setSize(const Size& size) {
        _size = size;
    }

    /**
     The offset of child geometries relative to the origin.
     */
    const Point contentOffset() const {
        return _contentOffset;
    }
    void setContentOffset(const Point& offset) {
        _contentOffset = offset;
    }

    const Rect bounds() const {
        return Rect(contentOffset(), size());
    }
    void setBounds(const Rect& bounds) {
        setSize(bounds.size);
        setContentOffset(bounds.origin);
    }

    /**
     The origin of the geometry in the parent geometry's coordinate system.
     */
    const Point origin() const {
        Point origin = location();
        origin.x -= anchorPoint().x;
        origin.y -= anchorPoint().y;
        return origin;
    }
    void setOrigin(const Point& origin) {
        _location = {
            origin.x + anchorPoint().x,
            origin.y + anchorPoint().y};
    }

    /**
     The center of the geometry in the parent geometry's coordinate system.
     */
    const Point center() const {
        Point center = origin();
        center.x += _size.width/2;
        center.y += _size.height/2;
        return center;
    }
    void setCenter(const Point& center) {
        setOrigin({center.x - _size.width/2, center.y - _size.height/2});
    }

    /**
     The frame of the geometry in the parent geometry's coordinate system.
     */
    Rect frame() const {
        return Rect(origin(), size());
    }
    void setFrame(const Rect& frame) {
        setSize(frame.size);
        setOrigin(frame.origin);
    }

    Geometry* parentGeometry() const {
        return _parentGeometry;
    }
    const Geometry* rootGeometry() const;

    /**
     Get all geometries
     */
    const std::vector<std::unique_ptr<Geometry>>& geometries() const {
        return _geometries;
    }

    /**
     Recursively looks up all geometries for the given type and calls the given function for
     each geometry found
     
     @param type The type to look up
     @param f The function to call for geometry found
     */
    void lookUpGeometriesWithTypes(const std::vector<std::type_index>& type, std::function<void (const Geometry* geometry)> f) const;

    void addGeometry(std::unique_ptr<Geometry>&& geom);
    
    /** The smallest rectangle that contains all sub-geometries. */
    Rect subGeometriesFrame() const;

    /**
     Get the first geometry that collides with a given rectangle.

     @param frame The frame to test for collisioins, in local coordinates.
     @return The colliding sub-geometry or nullptr if there are no collisions.
     */
    Geometry* collidingGeometry(const Rect& frame) const;

    /**
     Get all geometries that collide with a given rectangle.

     @param frame The frame to test for collisioins, in local coordinates.
     @return A vector of colliding sub-geometries.
     */
    std::vector<Geometry*> collidingGeometries(const Rect& frame) const;

    /**
     Convert a point from local coordinates to the parent geometry's coordinates.
     */
    Point convertToParent(const Point& point) const;

    /**
     Convert a point from the parent geometry's coordinates to local coordinates.
     */
    Point convertFromParent(const Point& point) const;

    /**
     Convert a point from local coordinates to the root geometry's coordinates.
     */
    Point convertToRoot(Point point) const;

    /**
     Convert a point from the root geometry's coordinates to local coordinates.
     */
    Point convertFromRoot(Point point) const;

    /**
     Convert a point from local coordinates to a geometry's coordinates.
     */
    Point convertToGeometry(Point point, const Geometry* target) const;

    /**
     Convert a point from a geometry's coordinates to local coordinates.
     */
    Point convertFromGeometry(Point point, const Geometry* target) const;

    /**
     Convert a rectangle from local coordinates to the parent geometry's coordinates.
     */
    Rect convertToParent(const Rect& rect) const;

    /**
     Convert a rectangle from the parent geometry's coordinates to local coordinates.
     */
    Rect convertFromParent(const Rect& rect) const;

    /**
     Convert a rectangle from local coordinates to the root geometry's coordinates.
     */
    Rect convertToRoot(Rect rect) const;

    /**
     Convert a rectangle from the root geometry's coordinates to local coordinates.
     */
    Rect convertFromRoot(Rect rect) const;

    /**
     Convert a rectangle from local coordinates to a geometry's coordinates.
     */
    Rect convertToGeometry(Rect rect, const Geometry* target) const;

    /**
     Convert a rectangle from a geometry's coordinates to local coordinates.
     */
    Rect convertFromGeometry(Rect rect, const Geometry* target) const;

    /**
     Determines if the geometry is active or inactive. Inactive geometries are grayed out.
     */
    bool isActive() const {
        return _active;
    }
    void setActive(bool active);

protected:
    Point _contentOffset;
    Size _size;

    Point _location;
    coord_t _horizontalAnchorPointMultiplier;
    coord_t _horizontalAnchorPointConstant;
    coord_t _verticalAnchorPointMultiplier;
    coord_t _verticalAnchorPointConstant;

    Geometry* _parentGeometry;
    std::vector<std::unique_ptr<Geometry>> _geometries;

    bool _active;
};

} // namespace mxml
