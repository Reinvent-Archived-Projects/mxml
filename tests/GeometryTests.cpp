// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <mxml/geometry/Geometry.h>
#include <mxml/StreamOperators.h>
#include <boost/test/unit_test.hpp>

using namespace mxml;

BOOST_AUTO_TEST_CASE(defaultValues) {
    Geometry geometry;
    BOOST_CHECK_EQUAL(geometry.location(), Point(0.0, 0.0));
    BOOST_CHECK_EQUAL(geometry.size(), Size(0.0, 0.0));
    BOOST_CHECK_EQUAL(geometry.origin(), Point(0.0, 0.0));
    BOOST_CHECK_EQUAL(geometry.center(), Point(0.0, 0.0));
    BOOST_CHECK_EQUAL(geometry.frame(), Rect(Point(0.0, 0.0), Size(0.0, 0.0)));
    BOOST_CHECK_EQUAL(geometry.contentOffset(), Point(0.0, 0.0));
}

// The following test cases use this geometry placement
//
//   0 1 2 3 4 5 6 7 8 9 0
// 0┌────────────────────
// 1│  ┌──────────────┐
// 2│  │              │
// 3│  │              │
// 4│  │              │
// 5│  │              │
// 6│  │              │
// 7│  │   *          │
// 8│  └──────────────┘
// 9│
//

void configureGeometry(Geometry& geometry) {
    geometry.setHorizontalAnchorPointValues(0.25, 0);
    geometry.setVerticalAnchorPointValues(0.75, 0);
    geometry.setLocation({30, 70});
    geometry.setSize({80, 80});
}

BOOST_AUTO_TEST_CASE(placement) {
    Geometry geometry;
    configureGeometry(geometry);

    BOOST_CHECK_EQUAL(geometry.origin(), Point(10, 10));
    BOOST_CHECK_EQUAL(geometry.center(), Point(50, 50));
    BOOST_CHECK_EQUAL(geometry.frame(), Rect(Point(10, 10), Size(80, 80)));
}

BOOST_AUTO_TEST_CASE(convertToParent) {
    Geometry parent;
    parent.setSize({200, 200});

    Geometry child;
    configureGeometry(child);

    BOOST_CHECK_EQUAL(child.convertToParent({0, 0}), child.origin());
    BOOST_CHECK_EQUAL(child.convertToParent({80, 80}), child.frame().max());
}

BOOST_AUTO_TEST_CASE(convertFromParent) {
    Geometry parent;
    parent.setSize({200, 200});

    Geometry child;
    configureGeometry(child);

    BOOST_CHECK_EQUAL(child.convertFromParent(child.origin()), Point(0, 0));
    BOOST_CHECK_EQUAL(child.convertFromParent(child.frame().max()), Point(80, 80));
}

BOOST_AUTO_TEST_CASE(convertToParentWithContentOffset) {
    Geometry parent;
    parent.setSize({200, 200});

    Geometry child;
    configureGeometry(child);
    child.setContentOffset({-10, 20});

    BOOST_CHECK_EQUAL(child.convertToParent({0, 0}), Point(20, -10));
}

BOOST_AUTO_TEST_CASE(convertFromParentWithContentOffset) {
    Geometry parent;
    parent.setSize({200, 200});

    Geometry child;
    configureGeometry(child);
    child.setContentOffset({-10, 20});

    BOOST_CHECK_EQUAL(child.convertFromParent({20, -10}), Point(0, 0));
}

BOOST_AUTO_TEST_CASE(convertPointToGeometryNull) {
    Geometry root;
    root.setSize({200, 200});

    Geometry* child;
    {
        std::unique_ptr<Geometry> geom(new Geometry);
        child = geom.get();
        configureGeometry(*child);
        geom->setContentOffset({-10, 20});
        root.addGeometry(std::move(geom));
    }

    Point result = child->convertToGeometry({0, 0}, nullptr);
    BOOST_CHECK_EQUAL(result, child->convertToParent({0, 0}));
}

BOOST_AUTO_TEST_CASE(convertPointToGeometrySelf) {
    Geometry root;
    root.setSize({200, 200});

    Geometry* child;
    {
        std::unique_ptr<Geometry> geom(new Geometry);
        child = geom.get();
        configureGeometry(*child);
        geom->setContentOffset({-10, 20});
        root.addGeometry(std::move(geom));
    }

    Point result = child->convertToGeometry({0, 0}, child);
    BOOST_CHECK_EQUAL(result, Point(0, 0));
}

BOOST_AUTO_TEST_CASE(convertPointToGeometryParent) {
    Geometry root;
    root.setSize({200, 200});

    Geometry* child;
    {
        std::unique_ptr<Geometry> geom(new Geometry);
        child = geom.get();
        configureGeometry(*child);
        geom->setContentOffset({-10, 20});
        root.addGeometry(std::move(geom));
    }

    Point result = child->convertToGeometry({0, 0}, &root);
    BOOST_CHECK_EQUAL(result, child->convertToParent({0, 0}));
}

BOOST_AUTO_TEST_CASE(convertPointToGeometrySibling) {
    Geometry root;
    root.setSize({200, 200});

    Geometry* child1;
    {
        std::unique_ptr<Geometry> geom(new Geometry);
        child1 = geom.get();
        configureGeometry(*child1);
        root.addGeometry(std::move(geom));
    }

    Geometry* child2;
    {
        std::unique_ptr<Geometry> geom(new Geometry);
        child2 = geom.get();
        configureGeometry(*child2);
        geom->setLocation({120, 70});
        root.addGeometry(std::move(geom));
    }

    Point result = child1->convertToGeometry({0, 0}, child2);
    BOOST_CHECK_EQUAL(result, Point(-90, 0));
}
