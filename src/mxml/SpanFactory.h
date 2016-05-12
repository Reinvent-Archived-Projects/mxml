// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once
#include "SpanCollection.h"

#include <mxml/ScoreProperties.h>
#include <mxml/dom/Barline.h>
#include <mxml/dom/Chord.h>
#include <mxml/dom/Direction.h>
#include <mxml/dom/Measure.h>
#include <mxml/dom/Score.h>

#include <memory>


namespace mxml {

/**
 SpanFactory builds a SpanCollection from a Score. This is the first layout pass.
 */
class SpanFactory {
public:
    SpanFactory(const dom::Score& score, const ScoreProperties& scoreProperties);

    /**
     With natural spacing enabled notes with longer durations have proportinaly more space to their right.
     */
    bool naturalSpacing() const {
        return _naturalSpacing;
    }
    void setNaturalSpacing(bool value) {
        _naturalSpacing = value;
    }

    /**
     Build the span collection for the whole score, used in a scroll layout.
     */
    std::unique_ptr<SpanCollection> build();
    
private:
    void build(const dom::Part* part, std::size_t beginMeasureIndex, std::size_t endMeasureIndex);
    void build(const dom::Measure* measure, bool buildMeasureAttributes);
    void build(const dom::Measure* measure);
    void build(const dom::Attributes* attributes);
    void build(const dom::Barline* barline);
    void build(const dom::Direction* direction);
    void build(const dom::TimedNode* node);
    void build(const dom::Chord* chord);
    void build(const dom::Note* note);
    void build(const dom::Clef* clefNode, int staff, int time);
    void build(const dom::Time* timeNode, int staff, int time);
    void build(const dom::Key* keyNode, int staff, int time);

    coord_t naturalWidthForNote(const dom::Note& note);
    SpanCollection::iterator graceNoteSpan(const dom::Chord* chord);

    void removeRedundantSpans();
    static bool isAttributeOnlySpan(const Span& span);

private:
    const dom::Score& _score;
    const ScoreProperties& _scoreProperties;
    bool _naturalSpacing;

    std::size_t _partIndex;
    std::size_t _measureIndex;
    dom::time_t _currentTime;
    dom::time_t _nextTime;

    std::unique_ptr<SpanCollection> _spans;
};

} // namespace mxml
