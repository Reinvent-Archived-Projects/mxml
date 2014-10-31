//  Created by Alejandro Isaza on 2014-04-30.
//  Copyright (c) 2014 Venture Media Labs. All rights reserved.

#include "SpanCollection.h"
#include "dom/TimedNode.h"
#include "dom/Types.h"
#include <algorithm>

namespace mxml {

SpanCollection::SpanCollection() : _naturalSpacing(true) {}

SpanCollection::iterator SpanCollection::first() {
    return _spans.begin();
}

SpanCollection::iterator SpanCollection::last() {
    if (_spans.empty())
        return _spans.end();
    return _spans.end() - 1;
}

std::pair<SpanCollection::iterator, SpanCollection::iterator> SpanCollection::range(std::size_t measureIndex) {
    Span testSpan;
    testSpan.setMeasureIndex(measureIndex);

    auto pair = std::equal_range(_spans.begin(), _spans.end(), testSpan, [](const Span& s1, const Span& s2) {
        return s1.measureIndex() < s2.measureIndex();
    });
    if (pair.first == pair.second)
        return std::make_pair(_spans.end(), _spans.end());
    return pair;
}

std::pair<SpanCollection::const_iterator, SpanCollection::const_iterator> SpanCollection::range(std::size_t measureIndex) const {
    Span testSpan;
    testSpan.setMeasureIndex(measureIndex);

    auto pair = std::equal_range(_spans.begin(), _spans.end(), testSpan, [](const Span& s1, const Span& s2) {
        return s1.measureIndex() < s2.measureIndex();
    });
    if (pair.first == pair.second)
        return std::make_pair(_spans.end(), _spans.end());
    return pair;
}

std::pair<SpanCollection::iterator, SpanCollection::iterator> SpanCollection::range(std::size_t measureIndex, int time) {
    Span testSpan;
    testSpan.setMeasureIndex(measureIndex);
    testSpan.setTime(time);

    auto pair = std::equal_range(_spans.begin(), _spans.end(), testSpan, [](const Span& s1, const Span& s2) {
        return s1.measureIndex() < s2.measureIndex() || (s1.measureIndex() == s2.measureIndex() && s1.time() < s2.time());
    });
    if (pair.first == pair.second)
        return std::make_pair(_spans.end(), _spans.end());
    return pair;
}

std::pair<SpanCollection::const_iterator, SpanCollection::const_iterator> SpanCollection::range(std::size_t measureIndex, int time) const {
    Span testSpan;
    testSpan.setMeasureIndex(measureIndex);
    testSpan.setTime(time);

    auto pair = std::equal_range(_spans.begin(), _spans.end(), testSpan, [](const Span& s1, const Span& s2) {
        return s1.measureIndex() < s2.measureIndex() || (s1.measureIndex() == s2.measureIndex() && s1.time() < s2.time());
    });
    if (pair.first == pair.second)
        return std::make_pair(_spans.end(), _spans.end());
    return pair;
}

SpanCollection::const_iterator SpanCollection::with(const dom::Node* node) const {
    auto it = _nodesMap.find(node);
    if (it == _nodesMap.end())
        return _spans.end();
    return _spans.begin() + it->second;
}

SpanCollection::iterator SpanCollection::with(const dom::Node* node) {
    auto it = _nodesMap.find(node);
    if (it == _nodesMap.end())
        return _spans.end();
    return _spans.begin() + it->second;
}

SpanCollection::iterator SpanCollection::eventSpan(std::size_t measureIndex, int time) {
    auto r = range(measureIndex, time);
    auto it = std::find_if(r.first, r.second, [](const Span& s) { return s.event(); });
    if (it == r.second)
        return end();
    return it;
}

SpanCollection::iterator SpanCollection::add(std::size_t measureIndex, int time) {
    Span testSpan;
    testSpan.setMeasureIndex(measureIndex);
    testSpan.setTime(time);

    iterator it = std::upper_bound(_spans.begin(), _spans.end(), testSpan, [](const Span& s1, const Span& s2){
        return s1.measureIndex() < s2.measureIndex() || (s1.measureIndex() == s2.measureIndex() && s1.time() < s2.time());
    });
    return _spans.insert(it, Span(measureIndex, time));
}

SpanCollection::iterator SpanCollection::addBeforeEvent(std::size_t measureIndex, int time) {
    Span testSpan;
    testSpan.setMeasureIndex(measureIndex);
    testSpan.setTime(time);

    iterator it = std::lower_bound(_spans.begin(), _spans.end(), testSpan, [](const Span& s1, const Span& s2){
        return s1.measureIndex() < s2.measureIndex() || (s1.measureIndex() == s2.measureIndex() && s1.time() < s2.time());
    });

    // Advance to the last span, or to the first event span
    while (it != _spans.end() && it->measureIndex() == measureIndex && it->time() == time && !it->event())
        ++it;

    return _spans.insert(it, Span(measureIndex, time));
}

coord_t SpanCollection::origin(std::size_t measureIndex) const {
    auto r = range(measureIndex);
    if (r.first == r.second)
        return 0;
    return r.first->start() - r.first->leftMargin();
}

coord_t SpanCollection::width(std::size_t measureIndex) const {
    coord_t width = 0;
    coord_t margin = 0;
    auto r = range(measureIndex);
    for (auto it = r.first; it != r.second; ++it) {
        auto& span = *it;
        margin = std::max(margin, span.leftMargin());
        if (_naturalSpacing)
            width += margin + std::max(span.width(), span.naturalWidth());
        else
            width += margin + span.width();
        margin = span.rightMargin();
    }
    width += margin;
    return width;
}

void SpanCollection::fillStarts() {
    coord_t width = 0;
    coord_t margin = 0;
    std::size_t measureIndex = 0;
    for (auto& span : _spans) {
        if (span.measureIndex() != measureIndex)
            margin += span.leftMargin(); // Double margin on measure boundaries
        else
            margin = std::max(margin, span.leftMargin());
        measureIndex = span.measureIndex();
        
        width += margin;
        span.setStart(width);

        if (_naturalSpacing)
            width += std::max(span.width(), span.naturalWidth());
        else
            width += span.width();
        margin = span.rightMargin();
    }
}

void SpanCollection::generateNodesMap() {
    _nodesMap.clear();
    for (std::size_t i = 0; i < _spans.size(); i += 1) {
        Span& span = _spans[i];
        for (const dom::Node* node : span.nodes()) {
            _nodesMap[node] = i;
        }
    }
}
    
} // namespace mxml
