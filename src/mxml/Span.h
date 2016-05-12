// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once
#include <mxml/geometry/Point.h>
#include <mxml/dom/Node.h>

#include <algorithm>
#include <set>
#include <typeinfo>


namespace mxml {

/**
 A Span is an interval of time along the score and stores placement information for all score elements in that
 interval. All score elements in a span need to align horizontally in the score. Spans are generated in the first
 layout pass and provide a means of making sure there is enough space for every element that needs to be drawn at each
 time point.
 
 There are annotation spans and event spans. Annotation spans have no notes in them, only annotations. Event spans have
 only notes. Spans store references to the score elements they contain as Node pointers.
 */
class Span {
public:
    Span() : _measureIndex(), _time(), _event(), _start(), _eventOffset(), _width(), _naturalWidth(-1), _leftMargin(), _rightMargin() {}
    Span(std::size_t measureIndex, int time) : _measureIndex(measureIndex), _time(time), _event(), _start(), _eventOffset(), _width(), _naturalWidth(-1), _leftMargin(), _rightMargin() {}
    
    int time() const {
        return _time;
    }
    void setTime(int time) {
        _time = time;
    }

    std::size_t measureIndex() const {
        return _measureIndex;
    }
    void setMeasureIndex(std::size_t m) {
        _measureIndex = m;
    }

    /** Determines if this is an even span (e.g. a note). */
    bool event() const {
        return _event;
    }
    void setEvent(bool event) {
        _event = event;
    }
    
    /** The start x coordinate for this span. */
    coord_t start() const {
        return _start;
    }
    void setStart(coord_t x) {
        _start = x;
    }
    
    /** The offset of the event body relative to start. */
    coord_t eventOffset() const {
        return _eventOffset;
    }
    void setEventOffset(coord_t offset) {
        _eventOffset = offset;
        _width = std::max(_width, offset);
    }
    void pushEventOffset(coord_t offset) {
        setEventOffset(std::max(_eventOffset, offset));
    }
    
    /** The span's width. */
    coord_t width() const {
        return _width;
    }
    coord_t end() const {
        return _start + _width;
    }
    void setWidth(coord_t width) {
        _eventOffset = std::min(_eventOffset, width);
        _width = width;
    }
    void pushWidth(coord_t width) {
        setWidth(std::max(_width, width));
    }

    /** The span's natural width: longer notes take more space. */
    coord_t naturalWidth() const {
        return _naturalWidth;
    }
    void setNaturalWidth(coord_t width) {
        _naturalWidth = width;
    }
    void pullNaturalWidth(coord_t width) {
        if (_naturalWidth == -1)
            _naturalWidth = width;
        else
            _naturalWidth = std::min(_naturalWidth, width);
    }
    
    coord_t leftMargin() const {
        return _leftMargin;
    }
    void setLeftMargin(coord_t margin) {
        _leftMargin = margin;
    }
    void pushLeftMargin(coord_t margin) {
        _leftMargin = std::max(_leftMargin, margin);
    }
    
    coord_t rightMargin() const {
        return _rightMargin;
    }
    void setRightMargin(coord_t margin) {
        _rightMargin = margin;
    }
    void pushRightMargin(coord_t margin) {
        _rightMargin = std::max(_rightMargin, margin);
    }
    
    bool hasNode(const dom::Node* node) const {
        return _nodes.count(node) > 0;
    }
    /** Determine if the span has a node of the given type */
    bool hasNodeType(const std::type_info& type) const;
    const std::set<const dom::Node*>& nodes() const {
        return _nodes;
    }
    void addNode(const dom::Node* node) {
        _nodes.insert(node);
    }
    
    bool operator==(const Span& rhs) const {
        return _time == rhs._time;
    }
    bool operator<(const Span& rhs) const {
        return _time < rhs._time;
    }
    bool operator!=(const Span& rhs) const {
        return !operator==(rhs);
    }
    bool operator<=(const Span& rhs) const {
        return operator<(rhs) || operator==(rhs);
    }
    bool operator>(const Span& rhs) const {
        return !operator<(rhs) && !operator==(rhs);
    }
    bool operator>=(const Span& rhs) const {
        return !operator<(rhs);
    }
    
private:
    std::size_t _measureIndex;;
    int _time;
    bool _event;
    
    coord_t _start;
    coord_t _eventOffset;
    coord_t _width;
    coord_t _naturalWidth;

    coord_t _leftMargin;
    coord_t _rightMargin;
    
    std::set<const dom::Node*> _nodes;
};

inline bool Span::hasNodeType(const std::type_info& type) const {
    for (auto it = _nodes.begin(); it != _nodes.end(); ++it) {
        const auto& nodeType = typeid(**it);
        if (nodeType == type)
            return true;
    }
    return false;
}

} // namespace mxml
