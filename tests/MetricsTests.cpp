
#include <mxml/ScoreBuilder.h>
#include <mxml/Metrics.h>

#include <boost/test/unit_test.hpp>

using namespace mxml;
using namespace dom;

BOOST_AUTO_TEST_CASE(noteStaffY) {
    ScoreBuilder builder;
    auto part = builder.addPart();
    auto measure = builder.addMeasure(part);
    auto attributes = builder.addAttributes(measure);
    attributes->setStaves(dom::presentOptional(2));
    builder.setTrebleClef(attributes, 1);
    builder.setBassClef(attributes, 2);

    auto score = builder.build();
    ScoreProperties scoreProperties(*score);

    Note note;
    note.setMeasure(measure);
    
    note.setPitch(std::unique_ptr<Pitch>(new Pitch(Pitch::STEP_G, 0, 4)));
    BOOST_CHECK_EQUAL(Metrics::staffY(scoreProperties, note), 30);
    
    note.setPitch(std::unique_ptr<Pitch>(new Pitch(Pitch::STEP_G, 0, 5)));
    BOOST_CHECK_EQUAL(Metrics::staffY(scoreProperties, note), -5);
    
    note.setPitch(std::unique_ptr<Pitch>(new Pitch(Pitch::STEP_G, 0, 3)));
    BOOST_CHECK_EQUAL(Metrics::staffY(scoreProperties, note), 65);
}

BOOST_AUTO_TEST_CASE(noteStaff2Y) {
    ScoreBuilder builder;
    auto part = builder.addPart();
    auto measure = builder.addMeasure(part);
    auto attributes = builder.addAttributes(measure);
    attributes->setStaves(dom::presentOptional(2));
    builder.setTrebleClef(attributes, 1);
    builder.setBassClef(attributes, 2);

    auto score = builder.build();
    ScoreProperties scoreProperties(*score);
    
    Note note;
    note.setStaff(2);
    note.setMeasure(measure);
    
    note.setPitch(std::unique_ptr<Pitch>(new Pitch(Pitch::STEP_F, 0, 3)));
    BOOST_CHECK_EQUAL(Metrics::staffY(scoreProperties, note), 10);
    
    note.setPitch(std::unique_ptr<Pitch>(new Pitch(Pitch::STEP_C, 0, 3)));
    BOOST_CHECK_EQUAL(Metrics::staffY(scoreProperties, note), 25);
}

BOOST_AUTO_TEST_CASE(staffOrigin) {
    Part part;
    
    BOOST_CHECK_EQUAL(Metrics::staffOrigin(part, 1), 0);
    BOOST_CHECK_EQUAL(Metrics::staffOrigin(part, 2), Metrics::staffHeight() + 65);
}

BOOST_AUTO_TEST_CASE(noteY) {
    ScoreBuilder builder;
    auto part = builder.addPart();
    auto measure = builder.addMeasure(part);
    auto attributes = builder.addAttributes(measure);
    attributes->setStaves(dom::presentOptional(2));
    builder.setTrebleClef(attributes, 1);
    builder.setBassClef(attributes, 2);

    auto score = builder.build();
    ScoreProperties scoreProperties(*score);
    
    Note note;
    note.setMeasure(measure);
    note.setStaff(1);
    note.setPitch(std::unique_ptr<Pitch>(new Pitch(Pitch::STEP_G, 0, 4)));
    BOOST_CHECK_EQUAL(Metrics::noteY(scoreProperties, note), 30);
    
    note.setStaff(2);
    note.setPitch(std::unique_ptr<Pitch>(new Pitch(Pitch::STEP_F, 0, 3)));
    BOOST_CHECK_EQUAL(Metrics::noteY(scoreProperties, note), Metrics::staffHeight() + 65 + 10);
}
