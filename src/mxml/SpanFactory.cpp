// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Algorithm.h"
#include "SpanFactory.h"

#include <mxml/geometry/AccidentalGeometry.h>
#include <mxml/geometry/BarlineGeometry.h>
#include <mxml/geometry/ChordGeometry.h>
#include <mxml/geometry/ClefGeometry.h>
#include <mxml/geometry/KeyGeometry.h>
#include <mxml/geometry/LyricGeometry.h>
#include <mxml/geometry/MeasureGeometry.h>
#include <mxml/geometry/NoteGeometry.h>
#include <mxml/geometry/StemGeometry.h>
#include <mxml/geometry/TimeSignatureGeometry.h>

#include <mxml/dom/Backup.h>
#include <mxml/dom/Forward.h>


namespace mxml {

static const coord_t kAttributeMargin = 10;
static const coord_t kRestWidth = 12;
static const coord_t kNoteMargin = 6;

SpanFactory::SpanFactory(const dom::Score& score, const ScoreProperties& scoreProperties)
: _score(score),
  _scoreProperties(scoreProperties),
  _naturalSpacing(false),
  _currentTime(0),
  _spans()
{
}

std::unique_ptr<SpanCollection> SpanFactory::build() {
    _spans.reset(new SpanCollection{_scoreProperties});
    _spans->setNaturalSpacing(_naturalSpacing);

    _partIndex = 0;
    for (auto& part : _score.parts()) {
        build(part.get(), 0, _scoreProperties.measureCount());
        _partIndex += 1;
    }
    removeRedundantSpans();

    _spans->normalizeChords();
    _spans->fillStarts();
    _spans->generateNodesMap();
    return std::move(_spans);
}

void SpanFactory::build(const dom::Part* part, std::size_t beginMeasureIndex, std::size_t endMeasureIndex) {
    for (_measureIndex = beginMeasureIndex; _measureIndex < endMeasureIndex; _measureIndex += 1) {
        _currentTime = 0;

        // In page layout mode we add attributes to the first measure in each system
        bool buildMeasureAttributes = false;
        if (_scoreProperties.layoutType() == ScoreProperties::LayoutType::Page) {
            auto systemIndex = _scoreProperties.systemIndex(_measureIndex);
            auto range = _scoreProperties.measureRange(systemIndex);
            buildMeasureAttributes = range.first == _measureIndex;
        }
        build(part->measures().at(_measureIndex).get(), buildMeasureAttributes);
    }
}

void SpanFactory::build(const dom::Measure* measure, bool buildMeasureAttributes) {
    if (buildMeasureAttributes)
        build(measure);

    auto& nodes = measure->nodes();
    for (auto it = nodes.begin(); it != nodes.end(); ++it) {
        auto& node = *it;

        // Compute next note time
        _nextTime = -1;
        for (auto it2 = std::next(it); it2 != nodes.end(); ++it2) {
            if (auto chord = dynamic_cast<const dom::Chord*>(it2->get())) {
                _nextTime = chord->start();
                break;
            } else if (auto note = dynamic_cast<const dom::Note*>(it2->get())) {
                _nextTime = note->start();
                break;
            }
        }
        if (_nextTime == -1)
            _nextTime = _scoreProperties.divisionsPerMeasure(measure->index());

        if (const dom::Barline* barline = dynamic_cast<const dom::Barline*>(node.get())) {
            build(barline);
        } else if (const dom::Attributes* attributes = dynamic_cast<const dom::Attributes*>(node.get())) {
            build(attributes);
        } else if (const dom::Direction* direction = dynamic_cast<const dom::Direction*>(node.get())) {
            build(direction);
        } else if (const dom::TimedNode* timedNode = dynamic_cast<const dom::TimedNode*>(node.get())) {
            build(timedNode);
        }
    }

    auto measureRange = _spans->range(_measureIndex);
    if (measureRange.first == measureRange.second) {
        // Set measure padding for an empty measure
        auto span = _spans->add(_measureIndex, 0);
        span->pushLeftMargin(SpanCollection::kMeasureLeftPadding);
        span->pushRightMargin(SpanCollection::kMeasureRightPadding);
    } else {
        // Set measure padding unless the edge spans specifically want 0 margin (i.e. barlines)
        auto first = measureRange.first;
        if (first->leftMargin() > 0)
            first->pushLeftMargin(SpanCollection::kMeasureLeftPadding);

        auto last = measureRange.second;
        --last;
        if (last->rightMargin() > 0)
            last->pushRightMargin(SpanCollection::kMeasureRightPadding);
    }
}

void SpanFactory::build(const dom::Barline* barline) {
    dom::time_t time = _currentTime;
    const dom::Measure* measure = dynamic_cast<const dom::Measure*>(barline->parent());
    if (barline == measure->nodes().back().get())
        time = std::numeric_limits<int>::max();

    SpanCollection::iterator span = _spans->withType(_measureIndex, time, typeid(dom::Barline));
    if (span == _spans->end()) {
        span = _spans->add(_measureIndex, time);
        span->setEvent(false);
    }
    span->pushWidth(BarlineGeometry::Width(*barline));
    
    if (span->time() == 0) {
        span->setLeftMargin(0);
        span->setRightMargin(SpanCollection::kMeasureLeftPadding);
    } else if (span->time() == std::numeric_limits<int>::max()) {
        span->setLeftMargin(SpanCollection::kMeasureRightPadding);
        span->setRightMargin(-1);
    }
    
    span->addNode(barline);
}

void SpanFactory::build(const dom::Measure* measure) {
    const auto time = 0;
    for (int staff = 1; staff <= _scoreProperties.staves(_partIndex); ++staff) {
        auto clefNode = _scoreProperties.clef(_partIndex, _measureIndex, staff, time);
        auto keyNode = _scoreProperties.key(_partIndex, _measureIndex, staff, time);
        build(clefNode, staff, time);
        build(keyNode, staff, time);
    }
}

void SpanFactory::build(const dom::Attributes* attributes) {
    dom::time_t time = _nextTime;
    const dom::Measure* measure = dynamic_cast<const dom::Measure*>(attributes->parent());
    if (attributes == measure->nodes().back().get())
        time = std::numeric_limits<int>::max();

    for (int staff = 1; staff <= _scoreProperties.staves(_partIndex); ++staff) {
        auto clefNode = attributes->clef(staff);
        auto keyNode = attributes->key(staff);
        auto timeNode = attributes->time();
        build(clefNode, staff, time);
        build(keyNode, staff, time);
        build(timeNode, staff, time);
    }
}

void SpanFactory::build(const dom::Direction* direction) {
    _currentTime = direction->start();

    SpanCollection::iterator span = _spans->eventSpan(_measureIndex, _currentTime);
    if (span == _spans->end()) {
        span = _spans->add(_measureIndex, _currentTime);
        span->setEvent(true);
    }
    
    span->addNode(direction);
}

void SpanFactory::build(const dom::TimedNode* node) {
    _currentTime = node->start();
    
    if (auto chord = dynamic_cast<const dom::Chord*>(node)) {
        build(chord);
    } else if (auto note = dynamic_cast<const dom::Note*>(node)) {
        build(note);
    }
}

void SpanFactory::build(const dom::Chord* chord) {
    if (!chord->firstNote() || !chord->firstNote()->printObject)
        return;

    SpanCollection::iterator span;
    if (chord->firstNote()->grace()) {
        span = graceNoteSpan(chord);
    } else {
        span = _spans->eventSpan(_measureIndex, _currentTime);
        if (span == _spans->end()) {
            span = _spans->add(_measureIndex, _currentTime);
            span->setEvent(true);
        }
    }
    
    coord_t accidentalWidth = 0;
    coord_t headWidth = 0;
    coord_t stemWidth = 0;
    coord_t naturalWidth = -1;
    coord_t lyricsWidth = 0;
    for (auto& note : chord->notes()) {
        headWidth = std::max(headWidth, NoteGeometry::Size(*note).width);

        int alter = note->alter();
        int previousAlter = _scoreProperties.alter(*note);
        if (alter != previousAlter)
            accidentalWidth = AccidentalGeometry::Size(note->alter()).width;

        if (note->stem() == dom::Stem::Up && note->beams().empty() && chord->firstNote()->beams().empty())
            stemWidth = StemGeometry::Size(*note, dom::Stem::Up, true).width - StemGeometry::kNoFlagWidth;
        
        if (naturalWidth == -1)
            naturalWidth = naturalWidthForNote(*note);
        else
            naturalWidth = std::min(naturalWidth, naturalWidthForNote(*note));

        for (auto& lyric : note->lyrics()) {
            lyricsWidth = std::max(lyricsWidth, LyricGeometry::width(*lyric));
        }
    }
    
    coord_t width = headWidth;

    // The accidental should overlap the left margin
    if (accidentalWidth > 0) {
        accidentalWidth += ChordGeometry::kAccidentalSpacing * 2; // Make sure there is spacing on each side
        width += std::max(coord_t(0), accidentalWidth - kNoteMargin);
    }
    // The stem flag should overlap the right margin
    if (stemWidth > 0)
        width += std::max(coord_t(0), stemWidth - kNoteMargin);

    width = std::max(width, lyricsWidth);

    if (chord->firstNote()->grace()) {
        width *= MeasureGeometry::kGraceNoteScale;
        if (width > 0) {
            span->pushLeftMargin(kNoteMargin * MeasureGeometry::kGraceNoteScale);
            span->pushRightMargin(kNoteMargin * MeasureGeometry::kGraceNoteScale);
        }
        span->pushWidth(width * MeasureGeometry::kGraceNoteScale);
        span->pushEventOffset((accidentalWidth + headWidth/2) * MeasureGeometry::kGraceNoteScale);
    } else if (width > 0) {
        span->pushLeftMargin(kNoteMargin);
        span->pushRightMargin(kNoteMargin);
        span->pushWidth(width);
        span->pullNaturalWidth(naturalWidth);
        span->pushEventOffset(std::max(coord_t(0), accidentalWidth - kNoteMargin) + headWidth/2);
    }

    span->addNode(chord);
    for (auto& note : chord->notes())
        span->addNode(note.get());
}

void SpanFactory::build(const dom::Note* note) {
    assert(note->rest);

    if (!note->printObject)
        return;

    auto span = _spans->eventSpan(_measureIndex, _currentTime);
    if (span == _spans->end()) {
        span = _spans->add(_measureIndex, _currentTime);
        span->setEvent(true);
    }
    span->pushWidth(kRestWidth);
    span->pullNaturalWidth(naturalWidthForNote(*note));
    span->pushEventOffset(kRestWidth/2);
    span->pushLeftMargin(kNoteMargin);
    span->pushRightMargin(kNoteMargin);
    span->addNode(note);
}

void SpanFactory::build(const dom::Clef* clefNode, int staff, int time) {
    if (!clefNode)
        return;

    SpanCollection::iterator clefSpan = _spans->withType(_measureIndex, time, typeid(dom::Clef));
    if (clefSpan == _spans->end())
        clefSpan = _spans->addBeforeEvent(_measureIndex, time);
    clefSpan->setEvent(false);
    clefSpan->pushLeftMargin(kAttributeMargin);
    clefSpan->pushRightMargin(kAttributeMargin);
    clefSpan->pushWidth(ClefGeometry::kSize.width);
    clefSpan->addNode(clefNode);
}

void SpanFactory::build(const dom::Time* timeNode, int staff, int time) {
    if (!timeNode)
        return;

    SpanCollection::iterator timeSpan = _spans->withType(_measureIndex, time, typeid(dom::Time));
    if (timeSpan == _spans->end())
        timeSpan = _spans->addBeforeEvent(_measureIndex, time);

    coord_t width = TimeSignatureGeometry(*timeNode).size().width;
    if (width > 0) {
        timeSpan->pushLeftMargin(kAttributeMargin);
        timeSpan->pushWidth(width);
        timeSpan->pushRightMargin(kAttributeMargin);
    }

    timeSpan->setEvent(false);
    timeSpan->addNode(timeNode);
}

void SpanFactory::build(const dom::Key* keyNode, int staff, int time) {
    if (!keyNode)
        return;

    coord_t width = 0;
    auto activeKey = _scoreProperties.key(_partIndex, _measureIndex, staff, time-1);
    if (activeKey && activeKey->fifths() != 0 && keyNode->fifths() == 0) {
        width = KeyGeometry::keySize(*activeKey).width;
    } else if (keyNode->fifths() != 0) {
        width = KeyGeometry::keySize(*keyNode).width;
    }

    if (width <= 0)
        return;

    SpanCollection::iterator keySpan = _spans->withType(_measureIndex, time, typeid(dom::Key));
    if (keySpan == _spans->end())
        keySpan = _spans->addBeforeEvent(_measureIndex, time);
    keySpan->pushLeftMargin(kAttributeMargin);
    keySpan->pushWidth(width);
    keySpan->pushRightMargin(kAttributeMargin);
    keySpan->setEvent(false);
    keySpan->addNode(keyNode);
}

SpanCollection::iterator SpanFactory::graceNoteSpan(const dom::Chord* chord) {
    const auto range = _spans->range(_measureIndex, _currentTime);
    const int staff = chord->firstNote()->staff();
    SpanCollection::iterator span = _spans->end();

    // Count number of grace notes on the same staff
    auto count = std::count_if(range.first, range.second, [staff](const Span& s) {
        const std::set<const dom::Node*>& nodes = s.nodes();
        auto it = std::find_if(nodes.begin(), nodes.end(), [staff](const dom::Node* n) {
            const dom::Chord* chord = dynamic_cast<const dom::Chord*>(n);
            if (!chord)
                return false;

            auto note = chord->firstNote();
            return note->grace() && note->staff() == staff;
        });
        return it != nodes.end();
    });

    // Get grace note span matching the count
    std::size_t n = 0;
    for (auto it = range.first; it != range.second; ++it) {
        for (auto& node : it->nodes()) {
            const dom::Chord* chord = dynamic_cast<const dom::Chord*>(node);
            if (!chord)
                continue;

            auto note = chord->firstNote();
            if (note->grace() && note->staff() != staff) {
                if (n == count)
                    span = it;
                n += 1;
            }
        }
    }

    if (span == _spans->end())
        span = _spans->addBeforeEvent(_measureIndex, _currentTime); // New grace note span

    return span;
}

void SpanFactory::removeRedundantSpans() {
    if (_spans->begin() == _spans->end())
        return;

    std::size_t measureCount = 0;
    for (auto& part : _score.parts())
        measureCount = std::max(measureCount, part->measures().size());

    for (std::size_t index = 0; index < measureCount; index += 1) {
        auto r = _spans->range(index);
        if (r.first == r.second)
            break;

        // Skip measures consisting of attributes only
        if (std::all_of(r.first, r.second, [](const Span& span) { return isAttributeOnlySpan(span); }))
            continue;

        // Remove attribute-only spans at the end, if next measure starts with attributes only
        for (auto it = r.second; it != r.first; ) {
            --it;
            if (it->event())
                break;

            auto rn = _spans->range(index + 1);
            if (isAttributeOnlySpan(*it) && rn.first != rn.second && isAttributeOnlySpan(*rn.first))
                _spans->erase(it);
        }
    }
}

bool SpanFactory::isAttributeOnlySpan(const Span& span) {
    if (span.event() || span.nodes().size() == 0)
        return false;

    for (auto& node : span.nodes()) {
        if (!dynamic_cast<const dom::Clef*>(node) && !dynamic_cast<const dom::Time*>(node) && !dynamic_cast<const dom::Key*>(node)) {
            return false;
        }
    }
    return true;
}

coord_t SpanFactory::naturalWidthForNote(const dom::Note& note) {
    static const coord_t kBaseWidth = 12;
    static const coord_t kWidthIncrease = 20;

    switch (note.type().value()) {
        case dom::Note::Type::_1024th:
        case dom::Note::Type::_512th:
        case dom::Note::Type::_256th:
        case dom::Note::Type::_128th:
        case dom::Note::Type::_64th:
        case dom::Note::Type::_32nd:
        case dom::Note::Type::_16th:
            return kBaseWidth;

        case dom::Note::Type::Eighth:
            return kBaseWidth + kWidthIncrease;

        case dom::Note::Type::Quarter:
            return kBaseWidth + 2*kWidthIncrease;

        case dom::Note::Type::Half:
            return kBaseWidth + 4*kWidthIncrease;

        case dom::Note::Type::Whole:
            return kBaseWidth + 8*kWidthIncrease;

        case dom::Note::Type::Breve:
            return kBaseWidth + 16*kWidthIncrease;
            
        case dom::Note::Type::Long:
            return kBaseWidth + 32*kWidthIncrease;
            
        case dom::Note::Type::Maxima:
            return kBaseWidth + 64*kWidthIncrease;
    }
}

} // namespace mxml
