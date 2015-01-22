//  Created by Steve Hosking on 2014-12-22.
//  Copyright (c) 2014 Venture Media Labs. All rights reserved.

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
    Metrics(const dom::Score& score, const ScoreProperties& scoreProperties);

    /**
     Get the height of a single staff in tenths. This is always 40.
     */
    static dom::tenths_t staffHeight();

    /**
     Get the height in tenths of a number staves, given their staff distance.
     */
    static dom::tenths_t stavesHeight(std::size_t staves, dom::tenths_t staffDistance);

    /**
     Return the y position of a note, relative to the top line of the topmost staff.
     */
    virtual dom::tenths_t noteY(const dom::Note& note) const = 0;

    /**
     Return the vertical (y) position of the note relative to its staff. Positions start at the top line of the staff
     and increase by 10 for every staff line going down.
     */
    dom::tenths_t staffY(const dom::Note& note) const;

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
        std::size_t partIndex;
        const dom::Print* print;

        bool operator==(const PrintRef& rhs) const {
            return systemIndex == rhs.systemIndex && measureIndex == rhs.measureIndex && partIndex == rhs.partIndex && print == rhs.print;
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

            if (partIndex < rhs.partIndex)
                return true;
            if (partIndex > rhs.partIndex)
                return false;

            return print < rhs.print;
        }
    };

protected:
    void process(std::size_t partIndex, const dom::Measure& measure);
    void process(std::size_t partIndex, std::size_t measureIndex, const dom::Print& print);

protected:
    const dom::Score& _score;
    const ScoreProperties& _scoreProperties;

    std::set<PrintRef> _prints;
    std::size_t _systemCount;
    std::size_t _pageCount;
};

}
