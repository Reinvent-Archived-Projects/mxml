// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once
#include <mxml/ScoreProperties.h>
#include <mxml/dom/Clef.h>
#include <mxml/dom/Pitch.h>
#include <mxml/dom/Print.h>
#include <mxml/dom/Rest.h>
#include <mxml/dom/Note.h>


namespace mxml {
    
class Metrics {
public:
    static const dom::tenths_t kStaffLineSpacing;
    static const std::size_t kStaffLineCount;

public:
    Metrics(const dom::Score& score, const ScoreProperties& scoreProperties, std::size_t partIndex);

    const dom::Score& score() const {
        return _score;
    }

    std::size_t partIndex() const {
        return _partIndex;
    }

    /**
     Get the number of staves in this part.
     */
    std::size_t staves() const;

    /**
     Get the height of a single staff in tenths. This is always 40.
     */
    static dom::tenths_t staffHeight();

    /**
     Get the staff distance in a part.
     */
    virtual dom::tenths_t staffDistance() const = 0;

    /**
     Get the height in tenths of all staves in a part. This does not include padding above the top staff or
     below the bottom staff, therefore it is smaller than the geometry's height.
     */
    dom::tenths_t stavesHeight() const;

    /**
     Return the y position, in tenths, of the top line of a staff relative to the top line of the topmost staff.
     The origin of staff 1 is awlays 0.
     */
    dom::tenths_t staffOrigin(int staffNumber) const;

    /**
     Return the y position of a note, relative to the top line of the topmost staff.
     */
    dom::tenths_t noteY(const dom::Note& note) const;

    /**
     Return the vertical (y) position of the note relative to its staff. Positions start at the top line of the staff
     and increase by 10 for every staff line going down.
     */
    dom::tenths_t staffY(const dom::Note& note) const;

public:
    /**
     Get the height in tenths of a number staves, given their staff distance.
     */
    static dom::tenths_t stavesHeight(std::size_t staves, dom::tenths_t staffDistance);

    /**
     Return the y position of a pitch within the staff for a given clef.
     */
    static dom::tenths_t staffY(const dom::Clef& clef, const dom::Pitch& pitch);
    static dom::tenths_t staffY(const dom::Clef& clef, const dom::Rest& rest);
    static dom::tenths_t staffYInGClef(dom::Pitch::Step step, int octave);
    static dom::tenths_t staffYInCClef(dom::Pitch::Step step, int octave);
    static dom::tenths_t staffYInFClef(dom::Pitch::Step step, int octave);

protected:
    struct PrintRef {
        std::size_t systemIndex;
        std::size_t measureIndex;
        const dom::Print* print;

        bool operator==(const PrintRef& rhs) const {
            return systemIndex == rhs.systemIndex && measureIndex == rhs.measureIndex && print == rhs.print;
        }
        bool operator<(const PrintRef& rhs) const {
            if (systemIndex < rhs.systemIndex)
                return true;
            if (systemIndex > rhs.systemIndex)
                return false;

            if (measureIndex < rhs.measureIndex)
                return true;
            if (measureIndex > rhs.measureIndex)
                return false;

            return print < rhs.print;
        }
    };

protected:
    void process(const dom::Measure& measure);
    void process(std::size_t measureIndex, const dom::Print& print);

protected:
    const dom::Score& _score;
    const ScoreProperties& _scoreProperties;

    std::set<PrintRef> _prints;
    std::size_t _systemCount;
    std::size_t _pageCount;

    std::size_t _partIndex;
};

}
