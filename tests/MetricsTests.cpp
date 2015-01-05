#include <mxml/Metrics.h>

#include <boost/test/unit_test.hpp>

using namespace mxml;
using namespace dom;

std::unique_ptr<Part> buildPartWithMeasure() {
    std::unique_ptr<Part> part(new Part());
    
    std::unique_ptr<Measure> measure(new Measure());
    
    std::unique_ptr<Attributes> attributes(new Attributes());
    attributes->setStaves(presentOptional(2));
    attributes->setClef(1, Clef::trebleClef());
    attributes->setClef(2, Clef::bassClef());
    measure->addNode(std::move(attributes));
    measure->setParent(part.get());
    
    part->addMeasure(std::move(measure));
    
    return std::move(part);
}

BOOST_AUTO_TEST_CASE(noteStaffY) {
    std::unique_ptr<Part> part = buildPartWithMeasure();
    auto& measure = part->measures().front();
    
    AttributesManager attributesManager;
    attributesManager.addAllAttribute(*measure);
    
    Note note;
    note.setMeasure(measure.get());
    
    note.setPitch(std::unique_ptr<Pitch>(new Pitch(Pitch::STEP_G, 0, 4)));
    BOOST_CHECK_EQUAL(Metrics::staffY(attributesManager, note), 30);
    
    note.setPitch(std::unique_ptr<Pitch>(new Pitch(Pitch::STEP_G, 0, 5)));
    BOOST_CHECK_EQUAL(Metrics::staffY(attributesManager, note), -5);
    
    note.setPitch(std::unique_ptr<Pitch>(new Pitch(Pitch::STEP_G, 0, 3)));
    BOOST_CHECK_EQUAL(Metrics::staffY(attributesManager, note), 65);
}

BOOST_AUTO_TEST_CASE(noteStaff2Y) {
    std::unique_ptr<Part> part = buildPartWithMeasure();
    auto& measure = part->measures().front();
    
    AttributesManager attributesManager;
    attributesManager.addAllAttribute(*measure);
    
    Note note;
    note.setStaff(2);
    note.setMeasure(measure.get());
    
    note.setPitch(std::unique_ptr<Pitch>(new Pitch(Pitch::STEP_F, 0, 3)));
    BOOST_CHECK_EQUAL(Metrics::staffY(attributesManager, note), 10);
    
    note.setPitch(std::unique_ptr<Pitch>(new Pitch(Pitch::STEP_C, 0, 3)));
    BOOST_CHECK_EQUAL(Metrics::staffY(attributesManager, note), 25);
}

BOOST_AUTO_TEST_CASE(staffOrigin) {
    Part part;
    
    BOOST_CHECK_EQUAL(Metrics::staffOrigin(part, 1), 0);
    BOOST_CHECK_EQUAL(Metrics::staffOrigin(part, 2), Metrics::staffHeight() + 65);
}

BOOST_AUTO_TEST_CASE(noteY) {
    std::unique_ptr<Part> part = buildPartWithMeasure();
    auto& measure = part->measures().front();
    
    AttributesManager attributesManager;
    attributesManager.addAllAttribute(*measure);
    
    Note note;
    note.setMeasure(measure.get());
    note.setStaff(1);
    note.setPitch(std::unique_ptr<Pitch>(new Pitch(Pitch::STEP_G, 0, 4)));
    BOOST_CHECK_EQUAL(Metrics::noteY(attributesManager, note), 30);
    
    note.setStaff(2);
    note.setPitch(std::unique_ptr<Pitch>(new Pitch(Pitch::STEP_F, 0, 3)));
    BOOST_CHECK_EQUAL(Metrics::noteY(attributesManager, note), Metrics::staffHeight() + 65 + 10);
}
