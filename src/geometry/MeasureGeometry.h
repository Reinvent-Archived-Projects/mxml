//  Created by Alejandro Isaza on 2014-04-28.
//  Copyright (c) 2014 Venture Media Labs. All rights reserved.

#pragma once
#include "Geometry.h"
#include "SpanCollection.h"

#include "dom/Attributes.h"
#include "dom/Measure.h"
#include "dom/Barline.h"
#include "dom/Chord.h"
#include "dom/Direction.h"
#include "dom/TimedNode.h"

#include <memory>
#include <vector>

namespace mxml {

class PartGeometry;

class MeasureGeometry : public Geometry {
public:
    static const coord_t kGraceNoteScale;
    static const coord_t kVerticalPadding;

public:
    MeasureGeometry(const dom::Measure& measure, const PartGeometry& partGeometry, const SpanCollection& spans);
    
    const dom::Measure& measure() const {
        return _measure;
    }
    const PartGeometry& partGeometry() const {
        return _partGeometry;
    }
    
    const SpanCollection& spans() const {
        return _spans;
    }

    bool naturalSpacing() const {
        return _spans.naturalSpacing();
    }

    /**
     Return the vertical (y) position of the note relative to its staff. Positions
     start at the top line of the staff and increase by 10 for every staff
     line going down.
     */
    static coord_t staffY(const dom::Attributes& attributes, const dom::Note& note);
    
private:
    /** Return the y position of a pitch within the staff for a given clef.
     */
    static coord_t staffY(const dom::Clef& clef, const dom::Pitch& pitch);
    static coord_t staffY(const dom::Clef& clef, const dom::Rest& rest);
    static coord_t staffYInGClef(dom::Pitch::Step step, int octave);
    static coord_t staffYInCClef(dom::Pitch::Step step, int octave);
    static coord_t staffYInFClef(dom::Pitch::Step step, int octave);
    
    void build();
    void buildAttributes(const dom::Attributes* attributes);
    void buildBarline(const dom::Barline* barline);
    void buildTimedNode(const dom::TimedNode* barline);
    void buildChord(const dom::Chord* chord);
    void buildRest(const dom::Note* note);
    
private:
    const dom::Measure& _measure;
    const PartGeometry& _partGeometry;
    const SpanCollection& _spans;
    std::size_t _measureIndex;
    
    int _currentTime;
    dom::Attributes _currentAttributes;
};

} // namespace mxml
