
#include "ScoreFactory.h"
#include <mxml/Metrics.h>

#include <boost/test/unit_test.hpp>

using namespace mxml;
using namespace dom;

BOOST_AUTO_TEST_CASE(noteStaffY) {
    auto score = tests::ScoreFactory::buildScorePiano();
    auto& part = score->parts().at(0);
    auto& measure = part->measures().at(0);

    ScoreProperties scoreProperties(*score);

    Note note;
    note.setMeasure(measure.get());
    
    note.setPitch(std::unique_ptr<Pitch>(new Pitch(Pitch::STEP_G, 0, 4)));
    BOOST_CHECK_EQUAL(Metrics::staffY(scoreProperties, note), 30);
    
    note.setPitch(std::unique_ptr<Pitch>(new Pitch(Pitch::STEP_G, 0, 5)));
    BOOST_CHECK_EQUAL(Metrics::staffY(scoreProperties, note), -5);
    
    note.setPitch(std::unique_ptr<Pitch>(new Pitch(Pitch::STEP_G, 0, 3)));
    BOOST_CHECK_EQUAL(Metrics::staffY(scoreProperties, note), 65);
}

BOOST_AUTO_TEST_CASE(noteStaff2Y) {
    auto score = tests::ScoreFactory::buildScorePiano();
    auto& part = score->parts().at(0);
    auto& measure = part->measures().at(0);

    ScoreProperties scoreProperties(*score);
    
    Note note;
    note.setStaff(2);
    note.setMeasure(measure.get());
    
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
    auto score = tests::ScoreFactory::buildScorePiano();
    auto& part = score->parts().at(0);
    auto& measure = part->measures().at(0);

    ScoreProperties scoreProperties(*score);
    
    Note note;
    note.setMeasure(measure.get());
    note.setStaff(1);
    note.setPitch(std::unique_ptr<Pitch>(new Pitch(Pitch::STEP_G, 0, 4)));
    BOOST_CHECK_EQUAL(Metrics::noteY(scoreProperties, note), 30);
    
    note.setStaff(2);
    note.setPitch(std::unique_ptr<Pitch>(new Pitch(Pitch::STEP_F, 0, 3)));
    BOOST_CHECK_EQUAL(Metrics::noteY(scoreProperties, note), Metrics::staffHeight() + 65 + 10);
}
