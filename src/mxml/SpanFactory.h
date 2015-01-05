//  Created by Alejandro Isaza on 2014-04-30.
//  Copyright (c) 2014 Venture Media Labs. All rights reserved.

#pragma once
#include "SpanCollection.h"

#include <mxml/AttributesManager.h>
#include <mxml/dom/Barline.h>
#include <mxml/dom/Chord.h>
#include <mxml/dom/Direction.h>
#include <mxml/dom/Measure.h>
#include <mxml/dom/Score.h>

namespace mxml {

/**
 SpanFactory builds a SpanCollection from a Score. This is the first layout pass.
 */
class SpanFactory {
public:
    SpanFactory(const dom::Score& score, bool naturalSpacing);
    const SpanCollection& build();
    
private:
    void build(const dom::Part* part);
    void build(const dom::Measure* measure, AttributesManager* attributesManager);
    void build(const dom::Attributes* attributes, AttributesManager* attributesManager);
    void build(const dom::Barline* barline);
    void build(const dom::Direction* direction);
    void build(const dom::TimedNode* node);
    void build(const dom::Chord* chord);
    void build(const dom::Note* note);

    coord_t naturalWidthForNote(const dom::Note& note);
    SpanCollection::iterator graceNoteSpan(const dom::Chord* chord);

    void removeRedundantSpans();
    static bool isAttributeOnlySpan(const Span& span);

private:
    const dom::Score& _score;

    std::size_t _measureIndex;
    int _currentTime;
    
    SpanCollection _spans;
};

} // namespace mxml
