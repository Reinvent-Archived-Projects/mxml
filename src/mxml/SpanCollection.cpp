// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Algorithm.h"
#include "SpanCollection.h"
#include <mxml/dom/Chord.h>
#include <mxml/dom/Types.h>
#include <algorithm>

namespace mxml {

SpanCollection::SpanCollection(const ScoreProperties& scoreProperties)
: _scoreProperties(scoreProperties),
  _naturalSpacing(true)
{}

std::size_t SpanCollection::beginMeasureIndex() const {
    if (_spans.empty())
        return 0;
    return _spans.front().measureIndex();
}

std::size_t SpanCollection::endMeasureIndex() const {
    if (_spans.empty())
        return 0;
    return _spans.back().measureIndex() + 1;
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

std::pair<SpanCollection::iterator, SpanCollection::iterator> SpanCollection::range(std::size_t measureIndex, dom::time_t time) {
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

std::pair<SpanCollection::const_iterator, SpanCollection::const_iterator> SpanCollection::range(std::size_t measureIndex, dom::time_t time) const {
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

SpanCollection::const_iterator SpanCollection::with(const dom::Node* node, std::size_t measureIndex) const {
    auto r = range(measureIndex);
    for (auto it = r.first; it != r.second; ++it) {
        if (it->hasNode(node))
            return it;
    }
    return _spans.end();
}

SpanCollection::iterator SpanCollection::with(const dom::Node* node, std::size_t measureIndex) {
    auto r = range(measureIndex);
    for (auto it = r.first; it != r.second; ++it) {
        if (it->hasNode(node))
            return it;
    }
    return _spans.end();
}

SpanCollection::iterator SpanCollection::withType(std::size_t measureIndex, dom::time_t time, const std::type_info& type) {
    auto r = range(measureIndex, time);
    for (auto it = r.first; it != r.second; ++it) {
        if (it->hasNodeType(type))
            return it;
    }
    return end();
}

SpanCollection::const_iterator SpanCollection::withType(std::size_t measureIndex, dom::time_t time, const std::type_info& type) const {
    auto r = range(measureIndex, time);
    for (auto it = r.first; it != r.second; ++it) {
        if (it->hasNodeType(type))
            return it;
    }
    return end();
}


SpanCollection::const_iterator SpanCollection::closest(std::size_t measureIndex, dom::time_t time, const std::type_info& type) const {
    auto closest = end();
    dom::time_t smallestDelta = std::numeric_limits<dom::time_t>::max();

    auto r = range(measureIndex);
    for (auto it = r.first; it != r.second; ++it) {
        if (!it->hasNodeType(type))
            continue;

        auto delta = std::abs(it->time() - time);
        if (delta < smallestDelta) {
            smallestDelta = delta;
            closest = it;
        }
    }
    
    return closest;
}

SpanCollection::iterator SpanCollection::eventSpan(std::size_t measureIndex, dom::time_t time) {
    auto r = range(measureIndex, time);
    auto it = std::find_if(r.first, r.second, [](const Span& s) { return s.event(); });
    if (it == r.second)
        return end();
    return it;
}

SpanCollection::const_iterator SpanCollection::eventSpan(std::size_t measureIndex, dom::time_t time) const {
    auto r = range(measureIndex, time);
    auto it = std::find_if(r.first, r.second, [](const Span& s) { return s.event(); });
    if (it == r.second)
        return end();
    return it;
}

SpanCollection::iterator SpanCollection::add(std::size_t measureIndex, dom::time_t time) {
    Span testSpan;
    testSpan.setMeasureIndex(measureIndex);
    testSpan.setTime(time);

    iterator it = std::upper_bound(_spans.begin(), _spans.end(), testSpan, [](const Span& s1, const Span& s2){
        return s1.measureIndex() < s2.measureIndex() || (s1.measureIndex() == s2.measureIndex() && s1.time() < s2.time());
    });
    return _spans.insert(it, Span(measureIndex, time));
}

SpanCollection::iterator SpanCollection::addBeforeEvent(std::size_t measureIndex, dom::time_t time) {
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

void SpanCollection::fitToWidth(coord_t targetWidth, std::size_t beginMeasure, std::size_t endMeasure) {
    // Compute total width
    std::vector<coord_t> widths;
    coord_t totalWidth = 0;
    for (auto measureIndex = beginMeasure; measureIndex != endMeasure; measureIndex += 1) {
        const auto measureWidth = width(measureIndex);
        widths.push_back(measureWidth);
        totalWidth += measureWidth;
    }

    // Not much we can do if the measures don't fit
    if (totalWidth >= targetWidth)
        return;

    // Scale all widths
    const auto ratio = targetWidth / totalWidth;
    for (auto measureIndex = beginMeasure; measureIndex != endMeasure; measureIndex += 1) {
        widths[measureIndex - beginMeasure] *= ratio;
    }

    // Find the set of integer widths that add up to the total width exactly, minimizing the error
    std::vector<int> newWidths;
    integerSum(widths.begin(), widths.end(), std::inserter(newWidths, newWidths.begin()));

    for (auto measureIndex = beginMeasure; measureIndex != endMeasure; measureIndex += 1) {
        const auto measureWidth = width(measureIndex);
        const auto newMeasureWidth = newWidths[measureIndex - beginMeasure];
        const auto extraWidth = newMeasureWidth - measureWidth;
        const auto totalDivisions = _scoreProperties.divisionsPerMeasure(measureIndex);
        const auto widthPerDivision = extraWidth / totalDivisions;

        auto spanRange = range(measureIndex);
        for (auto it = spanRange.first; it != spanRange.second; ++it) {
            auto next = std::next(it);
            dom::time_t duration;
            if (it->time() == std::numeric_limits<int>::max())
                duration = 0;
            else if (next == spanRange.second || next->time() == std::numeric_limits<int>::max())
                duration = totalDivisions - it->time();
            else
                duration = next->time() - it->time();
            
            const auto added = duration * widthPerDivision;
            it->setRightMargin(it->rightMargin() + added);
        }
    }
}

void SpanCollection::fillStarts() {
    coord_t width = 0;
    coord_t margin = 0;
    std::size_t measureIndex = -1;
    for (auto& span : _spans) {
        if (span.measureIndex() != measureIndex) {
            margin += span.leftMargin();
        } else {
            margin = std::max(margin, span.leftMargin());
        }

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

void SpanCollection::normalizeChords() {
    coord_t maxWidth = 0;
    for (auto measureIndex = beginMeasureIndex(); measureIndex < endMeasureIndex(); measureIndex +=1) {
        auto spanRange = range(measureIndex);
        for (auto it = spanRange.first; it != spanRange.second; ++it) {
            if (it->hasNodeType(typeid(dom::Chord)))
                maxWidth = std::max(maxWidth, it->width());
        }
        for (auto it = spanRange.first; it != spanRange.second; ++it) {
            if (it->hasNodeType(typeid(dom::Chord)))
                it->setWidth(maxWidth);
        }
        maxWidth = 0;
    }
}
    
} // namespace mxml
