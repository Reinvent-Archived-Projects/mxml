// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <lxml/lxml.h>
#include <mxml/parsing/ScoreHandler.h>
#include <sstream>
#include <boost/test/unit_test.hpp>


using namespace mxml;
using namespace mxml::parsing;

BOOST_AUTO_TEST_CASE(parseMusicXML) {
    const char musicXML[] =
        "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
        "<!DOCTYPE score-partwise PUBLIC \"-//Recordare//DTD MusicXML 2.0 Partwise//EN\" \"http://www.musicxml.org/dtds/partwise.dtd\">\n"
        "<score-partwise>\n"
        "  <part-list>\n"
        "    <score-part id=\"P1\">\n"
        "      <part-name>Piano</part-name>\n"
        "    </score-part>\n"
        "  </part-list>\n"
        "  <part id=\"P1\">\n"
        "    <measure number=\"22\">\n"
        "      <attributes>\n"
        "        <divisions>24</divisions>\n"
        "        <key>\n"
        "          <fifths>0</fifths>\n"
        "          <mode>major</mode>\n"
        "        </key>\n"
        "        <time symbol=\"common\">\n"
        "          <beats>4</beats>\n"
        "          <beat-type>4</beat-type>\n"
        "        </time>\n"
        "        <staves>2</staves>\n"
        "        <clef number=\"1\">\n"
        "          <sign>G</sign>\n"
        "          <line>2</line>\n"
        "        </clef>\n"
        "        <clef number=\"2\">\n"
        "          <sign>F</sign>\n"
        "          <line>4</line>\n"
        "        </clef>\n"
        "      </attributes>\n"
        "      <note>\n"
        "        <pitch>\n"
        "          <step>C</step>\n"
        "          <octave>6</octave>\n"
        "        </pitch>\n"
        "        <duration>36</duration>\n"
        "        <voice>1</voice>\n"
        "        <type>quarter</type>\n"
        "        <dot/>\n"
        "        <stem>down</stem>\n"
        "        <staff>1</staff>\n"
        "      </note>\n"
        "    </measure>\n"
        "  </part>\n"
        "</score-partwise>";
    
    ScoreHandler handler;
    std::stringstream ss(musicXML);
    lxml::parse(ss, "test", handler);
    
    const dom::Score& score = *handler.result();
    BOOST_CHECK_EQUAL(score.parts().size(), 1);
    
    const dom::Part& part = *score.parts()[0];
    BOOST_CHECK_EQUAL(part.parent(), &score);
    BOOST_CHECK_EQUAL(part.measures().size(), 1);
    
    const dom::Measure& measure = *part.measures()[0];
    BOOST_CHECK_EQUAL(measure.parent(), &part);
    BOOST_CHECK_EQUAL(measure.index(), 0);
    BOOST_CHECK_EQUAL(measure.number(), "22");
    BOOST_CHECK_EQUAL(measure.nodes().size(), 2);
    
    const dom::Attributes& attributes = dynamic_cast<dom::Attributes&>(*measure.nodes()[0]);
    BOOST_CHECK_EQUAL(attributes.divisions(), 24);
    BOOST_CHECK_EQUAL(attributes.staves(), 2);
    
    const dom::Chord& chord = dynamic_cast<dom::Chord&>(*measure.nodes()[1]);
    BOOST_CHECK_EQUAL(chord.notes().size(), 1);
    
    const dom::Note& note = *chord.notes()[0];
    BOOST_CHECK(note.pitch);
    BOOST_CHECK(note.pitch->step() == dom::Pitch::Step::C);
    BOOST_CHECK_EQUAL(note.pitch->octave(), 6);
    BOOST_CHECK_EQUAL(note.duration(), 36);
    BOOST_CHECK(note.type() == dom::Note::Type::Quarter);
    BOOST_CHECK(note.stem() == dom::Stem::Down);
    BOOST_CHECK_EQUAL(note.staff(), 1);
    BOOST_CHECK(note.dot);
}