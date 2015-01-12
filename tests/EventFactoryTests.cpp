//  Created by Alejandro Isaza on 2014-05-20.
//  Copyright (c) 2014 Venture Media Labs. All rights reserved.

#include <lxml/lxml.h>
#include <mxml/parsing/ScoreHandler.h>
#include <mxml/EventFactory.h>

#include <fstream>
#include <boost/test/unit_test.hpp>
#include <unistd.h>

using namespace mxml;

 BOOST_AUTO_TEST_CASE(moonlight) {
     ScoreHandler handler;
     std::ifstream is("moonlight.xml");
     lxml::parse(is, "moonlight.xml", handler);
    
     const dom::Score& score = *handler.result();
     ScoreProperties scoreProperties(score);

     EventFactory factory(score, scoreProperties);
     auto events = factory.build();
    
     BOOST_CHECK_EQUAL(events->events().size(), 2857);
    
     BOOST_CHECK_EQUAL(*events->tempo(0), 160);
     BOOST_CHECK_EQUAL(*events->tempo(6359), 160);
     BOOST_CHECK_CLOSE(*events->tempo(6360), 80, 0.01);
    
     BOOST_CHECK_EQUAL(events->loops().size(), 1);
     BOOST_CHECK_EQUAL(events->loops()[0].begin, 480);
     BOOST_CHECK_EQUAL(events->loops()[0].count, 1);
 }
