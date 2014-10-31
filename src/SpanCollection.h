//  Created by Alejandro Isaza on 2014-04-30.
//  Copyright (c) 2014 Venture Media Labs. All rights reserved.

#pragma once
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
    
public:
    SpanCollection();
    
    bool naturalSpacing() const {
        return _naturalSpacing;
    }
    void setNaturalSpacing(bool naturalSpacing) {
        _naturalSpacing = naturalSpacing;
    }
    
    /** Get the first span. */
    iterator first();
    
    /** Get the last span. */
    iterator last();

    /** Get the range of spans with a given measure number. */
    std::pair<iterator, iterator> range(std::size_t measureIndex);

    /** Get the range of spans with a given measure number. */
    std::pair<const_iterator, const_iterator> range(std::size_t measureIndex) const;
    
    /** Get the range of spans with a given measure number and time. */
    std::pair<iterator, iterator> range(std::size_t measureIndex, int time);
    
    /** Get the range of spans with a given measure number and time. */
    std::pair<const_iterator, const_iterator> range(std::size_t measureIndex, int time) const;
    
    /** Get the first span that contains the given node. Const version. Returns 0 if there is no such span. You need
     to call generateNodesMap() when there are modifications or the result of this method will be invalid. */
    const_iterator with(const dom::Node* node) const;

    /** Get the first span that contains the given node. Returns 0 if there is no such span. You need to call
     generateNodesMap() when there are modifications or the result of this method will be invalid. */
    iterator with(const dom::Node* node);

    /** Get the first span for a given measure and time that has the template type T. */
    template <typename T>
    iterator withType(std::size_t measureIndex, int time) {
        auto r = range(measureIndex, time);
        for (auto it = r.first; it != r.second; ++it) {
            const std::set<const dom::Node*>& nodes = it->nodes();
            for (auto nodeIt = nodes.begin(); nodeIt != nodes.end(); ++nodeIt) {
                if (dynamic_cast<const T*>(*nodeIt))
                    return it;
            }
        }
        return end();
    }

    /** Get the first span for a given measure and time that has the template type T. const version. */
    template <typename T>
    const_iterator withType(std::size_t measureIndex, int time) const {
        auto r = range(measureIndex, time);
        for (auto it = r.first; it != r.second; ++it) {
            const std::set<const dom::Node*>& nodes = it->nodes();
            for (auto nodeIt = nodes.begin(); nodeIt != nodes.end(); ++nodeIt) {
                if (dynamic_cast<const T*>(*nodeIt))
                    return it;
            }
        }
        return end();
    }

    /** Get the first event span in the given measure, with the given time. Or end if it's not found. */
    iterator eventSpan(std::size_t measureIndex, int time);
    
    /** Add a new span for the given time and measure. */
    iterator add(std::size_t measureIndex, int time);

    /** Add a new non-event span for the given time. The span in inserted before any event spans. */
    iterator addBeforeEvent(std::size_t measureIndex, int time);

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
     Fill in the spans' start location based on their widths and margins. Call this after the first layout pass to
     place the spans on their final locations. This is the second layout pass.
     */
    void fillStarts();

    /**
     Generate a hash map from nodes to spans. Call this after generating the collection. This is required for the
     with() methods.
     */
    void generateNodesMap();
    
private:
    std::vector<Span> _spans;
    std::unordered_map<const dom::Node*, std::size_t> _nodesMap;
    bool _naturalSpacing;
};

} // namespace mxml
