// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once
#include "ScoreProperties.h"
#include "Span.h"

#include <unordered_map>
#include <vector>

namespace mxml {

/**
 A collection of Spans. This class provides helper methods to create spans in the first layout pass and methods to
 query the spans on sucessive passes. SpanCollection keeps spans sorted by measure number and time and keeps annotation
 spans before event spans.
 */
class SpanCollection {
public:
    typedef std::vector<Span>::iterator iterator;
    typedef std::vector<Span>::const_iterator const_iterator;

    static constexpr coord_t kMeasureLeftPadding = 12;
    static constexpr coord_t kMeasureRightPadding = 4;
    
public:
    explicit SpanCollection(const ScoreProperties& scoreProperties);
    
    bool naturalSpacing() const {
        return _naturalSpacing;
    }
    void setNaturalSpacing(bool naturalSpacing) {
        _naturalSpacing = naturalSpacing;
    }

    std::size_t beginMeasureIndex() const;
    std::size_t endMeasureIndex() const;

    /** Get the range of spans with a given measure number. */
    std::pair<iterator, iterator> range(std::size_t measureIndex);

    /** Get the range of spans with a given measure number. */
    std::pair<const_iterator, const_iterator> range(std::size_t measureIndex) const;
    
    /** Get the range of spans with a given measure number and time. */
    std::pair<iterator, iterator> range(std::size_t measureIndex, dom::time_t time);
    
    /** Get the range of spans with a given measure number and time. */
    std::pair<const_iterator, const_iterator> range(std::size_t measureIndex, dom::time_t time) const;
    
    /**
     Get the first span that contains the given node. Const version. Returns 0 if there is no such span. You need
     to call generateNodesMap() when there are modifications or the result of this method will be invalid.
     */
    const_iterator with(const dom::Node* node) const;

    /**
     Get the first span that contains the given node. Returns end() if there is no such span. You need to call
     generateNodesMap() when there are modifications or the result of this method will be invalid.
     */
    iterator with(const dom::Node* node);

    /**
     Get the first span that contains the given node, constrained to the given measureIndex. Const version. Returns
     end() if there is no such span.
     */
    const_iterator with(const dom::Node* node, std::size_t measureIndex) const;

    /**
     Get the first span that contains the given node, constrained to the given measureIndex. Returns end() if there is
     no such span.
     */
    iterator with(const dom::Node* node, std::size_t measureIndex);

    /** Get the first span for a given measure and time that has a node of the given type. */
    iterator withType(std::size_t measureIndex, dom::time_t time, const std::type_info& type);

    /** Get the first span for a given measure and time that has a node of the given type. const version. */
    const_iterator withType(std::size_t measureIndex, dom::time_t time, const std::type_info& type) const;

    /** Get the span in the given measure closest to the given time that has the given node type. */
    const_iterator closest(std::size_t measureIndex, dom::time_t time, const std::type_info& type) const;

    /** Get the first event span in the given measure, with the given time. Or end if it's not found. */
    iterator eventSpan(std::size_t measureIndex, dom::time_t time);

    /** Get the first event span in the given measure, with the given time. Or end if it's not found. */
    const_iterator eventSpan(std::size_t measureIndex, dom::time_t time) const;
    
    /** Add a new span for the given time and measure. */
    iterator add(std::size_t measureIndex, dom::time_t time);

    /** Add a new non-event span for the given time. The span in inserted before any event spans. */
    iterator addBeforeEvent(std::size_t measureIndex, dom::time_t time);

    void erase(const_iterator pos) {
        _spans.erase(pos);
    }
    
    /** Get all the spans. */
    const std::vector<Span>& spans() const {
        return _spans;
    }
    
    const Span& at(std::size_t index) const {
        return _spans.at(index);
    }
    Span& at(std::size_t index) {
        return _spans.at(index);
    }
    
    iterator begin() {
        return _spans.begin();
    }
    const_iterator begin() const {
        return _spans.begin();
    }
    iterator end() {
        return _spans.end();
    }
    const_iterator end() const {
        return _spans.end();
    }
    
    std::size_t size() const {
        return _spans.size();
    }
    
    void clear() {
        _spans.clear();
    }
    
    bool empty() const {
        return _spans.empty();
    }

    /** Get the origin of the measure. */
    coord_t origin(std::size_t measureIndex) const;
    
    /** Compute the total width of the measure based on span widths and margins. */
    coord_t width(std::size_t measureIndex) const;

    /**
     Expand spacing between notes to fill up each system to the given width.
     */
    void fitToWidth(coord_t width, std::size_t begnMeasure, std::size_t endMeasure);
    
    /**
     Fill in the spans' start location based on their widths and margins. Call this after the first layout pass to
     place the spans on their final locations. This is the second layout pass.
     */
    void fillStarts();

    /**
     Generate a hash map from nodes to spans. Call this after generating the collection. This is required for the
     with() methods.
     */
    void generateNodesMap();

    /**
     Sets chords in each measure to the same width, the width used is the largest chord width in the measure.
     */
    void normalizeChords();
    
private:
    const ScoreProperties& _scoreProperties;

    std::vector<Span> _spans;
    std::unordered_map<const dom::Node*, std::size_t> _nodesMap;
    bool _naturalSpacing;
};

} // namespace mxml
