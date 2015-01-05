//  Created by Steve Hosking on 2014-12-22.
//  Copyright (c) 2014 Venture Media Labs. All rights reserved.

#pragma once

#include <mxml/AttributesManager.h>

#include <mxml/dom/Clef.h>
#include <mxml/dom/Pitch.h>
#include <mxml/dom/Rest.h>
#include <mxml/dom/Note.h>
#include <mxml/dom/Part.h>

#include <mxml/geometry/Point.h>

namespace mxml {
    
class Metrics {
public:
    static const coord_t kStaffLineSpacing;
    static const std::size_t kStaffLineCount;
    
    /** Get the height of a single staff. */
    static coord_t staffHeight();
    
    /** Get the total height of all staves. This does not include padding above the top staff or below the bottom staff, therefore it is smaller than the geometry's height.
     */
    static coord_t stavesHeight(const dom::Part& part);
    
    /** Return the y position of the top line of a staff, relative to the top line of the topmost staff.
     */
    static coord_t staffOrigin(const dom::Part& part, int staffNumber);
    
    /** Return the y position of a note, relative to the top line of the topmost staff.
     */
    static coord_t noteY(const AttributesManager& attributesManager, const dom::Note& note);
    
    /**
     Return the vertical (y) position of the note relative to its staff. Positions
     start at the top line of the staff and increase by 10 for every staff
     line going down.
     */
    static coord_t staffY(const AttributesManager& attributesManager, const dom::Note& note);

    /**
     Return the y position of a pitch within the staff for a given clef.
     */
    static coord_t staffY(const dom::Clef& clef, const dom::Pitch& pitch);
    static coord_t staffY(const dom::Clef& clef, const dom::Rest& rest);
    static coord_t staffYInGClef(dom::Pitch::Step step, int octave);
    static coord_t staffYInCClef(dom::Pitch::Step step, int octave);
    static coord_t staffYInFClef(dom::Pitch::Step step, int octave);
};

}
