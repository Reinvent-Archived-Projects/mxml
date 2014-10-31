#include "geometry/PartGeometry.h"
#include <boost/test/unit_test.hpp>

using namespace mxml;
using namespace dom;

BOOST_AUTO_TEST_CASE(staffOrigin) {
    SpanCollection spans;
    Part part;
    PartGeometry geom(part, spans);
    
    BOOST_CHECK_EQUAL(geom.staffOrigin(1), 0);
    BOOST_CHECK_EQUAL(geom.staffOrigin(2), geom.staffHeight() + 65);
}

BOOST_AUTO_TEST_CASE(noteY) {
    SpanCollection spans;
    Part part;
    PartGeometry geom(part, spans);
    
    Attributes attributes;
    attributes.setStaves(presentOptional(2));
    attributes.setClef(1, presentOptional(Clef::trebleClef()));
    attributes.setClef(2, presentOptional(Clef::bassClef()));
    
    Note note;
    note.setStaff(1);
    note.setPitch(presentOptional(Pitch(Pitch::STEP_G, 0, 4)));
    BOOST_CHECK_EQUAL(geom.noteY(attributes, note), 30);
    
    note.setStaff(2);
    note.setPitch(presentOptional(Pitch(Pitch::STEP_F, 0, 3)));
    BOOST_CHECK_EQUAL(geom.noteY(attributes, note), geom.staffHeight() + 65 + 10);
}
