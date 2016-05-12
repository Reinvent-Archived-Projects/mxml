// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <mxml/dom/Attributes.h>
#include <mxml/dom/Note.h>

#include <boost/test/unit_test.hpp>

using namespace mxml::dom;

Note createNote(int octave, Pitch::Step step, int alter) {
    std::unique_ptr<Pitch> pitch(new Pitch());
    pitch->setOctave(octave);
    pitch->setStep(step);
    pitch->setAlter(alter);

    Note note;
    note.pitch = std::move(pitch);

    return note;
}

BOOST_AUTO_TEST_CASE(midiNumberMiddleC) {
    Note note = createNote(4, Pitch::Step::C, 0);
    BOOST_CHECK_EQUAL(note.midiNumber(), 60);
}

BOOST_AUTO_TEST_CASE(midiNumberA4) {
    Note note = createNote(4, Pitch::Step::A, 0);
    BOOST_CHECK_EQUAL(note.midiNumber(), 69);
}

BOOST_AUTO_TEST_CASE(midiNumberA0) {
    Note note = createNote(0, Pitch::Step::A, 0);
    BOOST_CHECK_EQUAL(note.midiNumber(), 21);
}

BOOST_AUTO_TEST_CASE(midiNumberC8) {
    Note note = createNote(8, Pitch::Step::C, 0);
    BOOST_CHECK_EQUAL(note.midiNumber(), 108);
}

BOOST_AUTO_TEST_CASE(midiNumberMiddleCSharp) {
    Note note = createNote(4, Pitch::Step::C, 1);
    BOOST_CHECK_EQUAL(note.midiNumber(), 61);
}

BOOST_AUTO_TEST_CASE(midiNumberE4Flat) {
    Note note = createNote(4, Pitch::Step::E, -1);
    BOOST_CHECK_EQUAL(note.midiNumber(), 63);
}
