#include <mxml/geometry/MeasureGeometry.h>
#include <mxml/geometry/PartGeometry.h>
#include <mxml/SpanFactory.h>
#include <boost/test/unit_test.hpp>

using namespace mxml;
using namespace dom;

BOOST_AUTO_TEST_CASE(noteStaffY) {
    SpanCollection spans;
    Part part;
    PartGeometry partGeom(part, spans);

    Measure* measure = new Measure;
    part.addMeasure(std::unique_ptr<Measure>(measure));
    MeasureGeometry measureGeom(*measure, partGeom, spans);
    
    Attributes attributes;
    attributes.setStaves(presentOptional(1));
    attributes.setClef(1, presentOptional(Clef::trebleClef()));
    
    Note note;
    note.setPitch(presentOptional(Pitch(Pitch::STEP_G, 0, 4)));
    BOOST_CHECK_EQUAL(measureGeom.staffY(attributes, note), 30);
    
    note.setPitch(presentOptional(Pitch(Pitch::STEP_G, 0, 5)));
    BOOST_CHECK_EQUAL(measureGeom.staffY(attributes, note), -5);
    
    note.setPitch(presentOptional(Pitch(Pitch::STEP_G, 0, 3)));
    BOOST_CHECK_EQUAL(measureGeom.staffY(attributes, note), 65);
}

BOOST_AUTO_TEST_CASE(noteStaff2Y) {
    SpanCollection spans;
    Part part;
    PartGeometry partGeom(part, spans);

    Measure* measure = new Measure;
    part.addMeasure(std::unique_ptr<Measure>(measure));
    MeasureGeometry measureGeom(*measure, partGeom, spans);
    
    Attributes attributes;
    attributes.setStaves(presentOptional(2));
    attributes.setClef(1, presentOptional(Clef::trebleClef()));
    attributes.setClef(2, presentOptional(Clef::bassClef()));
    
    Note note;
    note.setStaff(2);
    note.setPitch(presentOptional(Pitch(Pitch::STEP_F, 0, 3)));
    BOOST_CHECK_EQUAL(measureGeom.staffY(attributes, note), 10);
    
    note.setPitch(presentOptional(Pitch(Pitch::STEP_C, 0, 3)));
    BOOST_CHECK_EQUAL(measureGeom.staffY(attributes, note), 25);
}
