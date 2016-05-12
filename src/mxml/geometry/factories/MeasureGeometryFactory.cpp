// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "MeasureGeometryFactory.h"

#include <mxml/Metrics.h>
#include <mxml/dom/Backup.h>
#include <mxml/dom/Forward.h>
#include <mxml/geometry/BarlineGeometry.h>
#include <mxml/geometry/BeamGeometry.h>
#include <mxml/geometry/ChordGeometry.h>
#include <mxml/geometry/ClefGeometry.h>
#include <mxml/geometry/KeyGeometry.h>
#include <mxml/geometry/NoteGeometry.h>
#include <mxml/geometry/RestGeometry.h>
#include <mxml/geometry/TimeSignatureGeometry.h>
#include <mxml/geometry/TupletGeometry.h>

#include "StemDirectionResolver.h"


namespace mxml {

using namespace dom;

MeasureGeometryFactory::MeasureGeometryFactory(const SpanCollection& spans,
                                               const ScoreProperties& scoreProperties,
                                               const Metrics& metrics)
: _spans(spans),
  _scoreProperties(scoreProperties),
  _metrics(metrics),
  _partIndex(metrics.partIndex()),
  _chordGeometryFactory(scoreProperties, metrics)
{
}

std::unique_ptr<MeasureGeometry> MeasureGeometryFactory::build(const Measure& measure, bool firstMeasureInSystem) {
    _measureIndex = measure.index();
    _geometry.reset(new MeasureGeometry(measure, _spans, _scoreProperties, _metrics));
    _currentTime = 0;

    if (firstMeasureInSystem)
        buildAttributes();

    if (firstMeasureInSystem || _scoreProperties.layoutType() == ScoreProperties::LayoutType::Scroll)
        _geometry->_showNumber = true;

    for (auto& node : measure.nodes()) {
        if (const Attributes* attributes = dynamic_cast<const Attributes*>(node.get())) {
            buildAttributes(attributes);
        } else if (const Barline* barline = dynamic_cast<const Barline*>(node.get())) {
            buildBarline(barline);
        } else if (const TimedNode* timedNode = dynamic_cast<const TimedNode*>(node.get())) {
            buildTimedNode(timedNode);
        }
    }

    StemDirectionResolver resolver;
    resolver.resolve(_geometry.get());

    // Fix chords for new stem directions
    for (std::size_t i = 0; i < _geometry->_geometries.size(); i += 1) {
        auto& geom = _geometry->_geometries[i];
        ChordGeometry* chordGeom = dynamic_cast<ChordGeometry*>(geom.get());
        if (chordGeom) {
            _chordGeometryFactory.resetForStem(chordGeom);
            placeChord(chordGeom);
        }
    }

    buildBeams();
    adjustBounds(_geometry.get());
    centerWholeRests();
    _tupletStart.clear();

    return std::move(_geometry);
}

void MeasureGeometryFactory::buildAttributes() {
    const auto time = 0;
    for (int staff = 1; staff <= _metrics.staves(); staff += 1) {
        auto clef = _scoreProperties.clef(_partIndex, _measureIndex, staff, time);
        auto key = _scoreProperties.key(_partIndex, _measureIndex, staff, time);

        if (buildClefGeometry(clef, staff))
            builtGeometries.insert(clef);

        if (buildKeyGeometry(key, staff, time))
            builtGeometries.insert(key);
    }
}

void MeasureGeometryFactory::buildAttributes(const Attributes* attributes) {
    for (int staff = 1; staff <= _metrics.staves(); staff += 1) {
        auto time = attributes->time();
        auto clef = attributes->clef(staff);
        auto key = attributes->key(staff);
        if (!key && staff > 1)
            key = attributes->key(1);

        if (builtGeometries.count(clef) == 0)
            buildClefGeometry(clef, staff);

        if (builtGeometries.count(key) == 0)
            buildKeyGeometry(key, staff, attributes->start());

        buildTimeGeometry(time, staff);
    }
}

bool MeasureGeometryFactory::buildClefGeometry(const dom::Clef* clef, int staff) {
    if (!clef)
        return false;

    auto it = _spans.with(clef, _measureIndex);
    if (it == _spans.end())
        return false;

    std::unique_ptr<ClefGeometry> geo(new ClefGeometry(*clef));
    geo->setStaff(staff);

    const Span& span = *it;
    Point location;
    location.x = span.start() + span.width()/2 - _spans.origin(_measureIndex);
    location.y = _metrics.staffOrigin(staff) + Metrics::staffHeight()/2;
    geo->setLocation(location);

    _geometry->addGeometry(std::move(geo));

    return true;
}

bool MeasureGeometryFactory::buildKeyGeometry(const dom::Key* key, int staff, dom::time_t time) {
    if (!key)
        return false;

    auto it = _spans.with(key, _measureIndex);
    if  (it == _spans.end())
        return false;

    const Span& span = *it;
    std::unique_ptr<KeyGeometry> geo;

    auto clef = _scoreProperties.clef(_partIndex, _measureIndex, staff, time);
    if (key->fifths() == 0) {
        const auto& activeKey = _scoreProperties.key(_partIndex, _measureIndex, staff, time-1);
        if (!activeKey)
            return false;

        geo.reset(new KeyGeometry(*activeKey, *clef));
        geo->setNatural(true);
    } else {
        geo.reset(new KeyGeometry(*key, *clef));
    }

    Point location;
    location.x = span.start() + span.width()/2 - _spans.origin(_measureIndex);
    location.y = _metrics.staffOrigin(staff) + Metrics::staffHeight()/2;
    geo->setLocation(location);
    geo->setStaff(staff);
    _geometry->addGeometry(std::move(geo));

    return true;
}

bool MeasureGeometryFactory::buildTimeGeometry(const dom::Time* time, int staff) {
    if (!time)
        return false;

    auto it = _spans.with(time, _measureIndex);
    if  (it == _spans.end())
        return false;

    const Span& span = *it;

    std::unique_ptr<TimeSignatureGeometry> geo(new TimeSignatureGeometry(*time));
    geo->setStaff(staff);

    Point location;
    location.x = span.start() + span.width()/2 - _spans.origin(_measureIndex);
    location.y = _metrics.staffOrigin(staff) + Metrics::staffHeight()/2;
    geo->setLocation(location);
    _geometry->addGeometry(std::move(geo));

    return true;
}

void MeasureGeometryFactory::buildBarline(const Barline* barline) {
    std::unique_ptr<BarlineGeometry> geo(new BarlineGeometry(*barline, _metrics));
    geo->setVerticalAnchorPointValues(0, BarlineGeometry::kLightLineWidth/2);
    geo->setHorizontalAnchorPointValues(0, BarlineGeometry::kLightLineWidth/2);

    auto it = _spans.with(barline);
    assert(it != _spans.end());

    const Span& span = *it;
    geo->setLocation({span.start() - _spans.origin(_measureIndex), 0});

    _geometry->addGeometry(std::move(geo));
}

void MeasureGeometryFactory::buildTimedNode(const TimedNode* node)  {
    if (auto forward = dynamic_cast<const Forward*>(node)) {
        _currentTime += forward->duration();
        return;
    }

    if (auto backup = dynamic_cast<const Backup*>(node)) {
        _currentTime -= backup->duration();
        if (_currentTime < 0)
            _currentTime = 0;
        return;
    }

    if (_currentTime != node->start()) {
        _currentTime = node->start();
    }

    if (auto chord = dynamic_cast<const Chord*>(node)) {
        buildChord(chord);
    } else if (auto note = dynamic_cast<const Note*>(node)) {
        buildRest(note);
    }
}

void MeasureGeometryFactory::buildChord(const Chord* chord) {
    auto geo = _chordGeometryFactory.build(*chord);
    if (!geo)
        return;

    placeChord(geo.get());

    for (auto& note : chord->notes()) {
        auto& notations = note->notations;
        if (!notations)
            continue;

        for (auto& tuplet : notations->tuplets) {
            buildTuplet(geo.get(), tuplet.get());
        }
    }

    _geometry->addGeometry(std::move(geo));
}

void MeasureGeometryFactory::buildTuplet(const ChordGeometry* chordGeometry, const dom::Tuplet* tuplet) {
    static constexpr coord_t kStaffSpacing = 2;

    if (tuplet->type == dom::Tuplet::Type::Start) {
        _tupletStart.insert(std::make_pair(tuplet->number, std::make_pair(chordGeometry, tuplet)));
        return;
    }

    auto it = _tupletStart.find(tuplet->number);
    if (it == _tupletStart.end()) {
        // Tuplet has an end but not a start?!
        return;
    }

    const auto startChordGeometry = it->second.first;
    const auto startTuplet = it->second.second;
    const auto stopChordGeometry = chordGeometry;
    const auto stopTuplet = tuplet;
    const auto staff = startChordGeometry->chord().firstNote()->staff();

    auto placement = dom::Placement::Above;
    if (startTuplet->placement.isPresent())
        placement = startTuplet->placement;
    else if (stopTuplet->placement.isPresent())
        placement = stopTuplet->placement;
    else if (startChordGeometry->stem() && startChordGeometry->stem()->stemDirection() == dom::Stem::Down &&
             stopChordGeometry->stem() && stopChordGeometry->stem()->stemDirection() == dom::Stem::Down)
        placement = dom::Placement::Below;

    Point startLocation;
    Point stopLocation;
    startLocation.x = startChordGeometry->frame().min().x;
    stopLocation.x = stopChordGeometry->frame().max().x;
    if (placement == dom::Placement::Above) {
        startLocation.y = _metrics.staffOrigin(staff) - kStaffSpacing;
        stopLocation.y = startLocation.y;
    } else {
        startLocation.y = _metrics.staffOrigin(staff) + _metrics.staffHeight() + kStaffSpacing;
        stopLocation.y = startLocation.y;
    }

    auto tupletGeometry = std::unique_ptr<TupletGeometry>(new TupletGeometry{*startTuplet, startLocation, *stopTuplet, stopLocation, placement, staff});

    if (startTuplet->actual.number.isPresent())
        tupletGeometry->setDisplayNumber(startTuplet->actual.number);
    else if (auto& timeModification = startChordGeometry->chord().firstNote()->timeModification)
        tupletGeometry->setDisplayNumber(timeModification->actualNotes);

    if (startTuplet->bracket.isPresent())
        tupletGeometry->setBracket(startTuplet->bracket.value());
    else if (stopTuplet->bracket.isPresent())
        tupletGeometry->setBracket(stopTuplet->bracket.value());
    else
        tupletGeometry->setBracket(!startChordGeometry->chord().hasBeam() || !stopChordGeometry->chord().hasBeam());
    
    _geometry->addGeometry(std::move(tupletGeometry));
    _tupletStart.erase(it);
}

void MeasureGeometryFactory::placeChord(ChordGeometry* chordGeom) {
    auto refLocation = chordGeom->refNoteLocation();
    auto stemDirection = dom::Stem::Up;
    if (chordGeom->stem())
        stemDirection = chordGeom->stem()->stemDirection();

    if (stemDirection == dom::Stem::Up) {
        chordGeom->setHorizontalAnchorPointValues(0, refLocation.x - chordGeom->contentOffset().x);
        chordGeom->setVerticalAnchorPointValues(1, -(chordGeom->size().height - (refLocation.y - chordGeom->contentOffset().y)));
    } else {
        chordGeom->setHorizontalAnchorPointValues(1, -(chordGeom->size().width - (refLocation.x - chordGeom->contentOffset().x)));
        chordGeom->setVerticalAnchorPointValues(0, refLocation.y - chordGeom->contentOffset().y);
    }

    auto& chord = chordGeom->chord();
    auto it = _spans.with(&chord);
    assert(it != _spans.end());

    const Span& span = *it;
    Point location = chordGeom->refNoteLocation();
    if (chord.firstNote()->grace()) {
        location.x = span.start() - _spans.origin(_measureIndex) + chordGeom->anchorPoint().x;
    } else {
        location.x = span.start() + span.eventOffset() - _spans.origin(_measureIndex);
    }
    chordGeom->setLocation(location);
}

void MeasureGeometryFactory::buildRest(const Note* note) {
    assert(note->rest);
    if (!note->printObject)
        return;

    std::unique_ptr<RestGeometry> geo(new RestGeometry(*note));

    auto it = _spans.with(note);
    assert(it != _spans.end());

    const Span& span = *it;
    Point location;
    location.x = span.start() + span.eventOffset() - _spans.origin(_measureIndex);
    location.y = _metrics.noteY(*note);
    geo->setLocation(location);
    _geometry->addGeometry(std::move(geo));
}

void MeasureGeometryFactory::centerWholeRests() {
    for (auto& geometry : _geometry->geometries()) {
        RestGeometry* rest = dynamic_cast<RestGeometry*>(geometry.get());
        if (!rest)
            continue;

        if (!rest->note().type().isPresent() || rest->note().type() == dom::Note::Type::Whole) {
            auto location = rest->location();
            const Span& span = *_spans.with(&rest->note());
            auto start = -_spans.origin(_measureIndex) + span.start();
            location.x = start + (_geometry->size().width - start) / 2 - rest->size().width/2;
            rest->setLocation(location);
        }
    }
}

void MeasureGeometryFactory::buildBeams() {
    std::map<std::string, std::vector<ChordGeometry*>> chordsByVoice;

    for (std::size_t i = 0; i < _geometry->_geometries.size(); i += 1) {
        auto& geom = _geometry->_geometries[i];
        ChordGeometry* chordGeom = dynamic_cast<ChordGeometry*>(geom.get());
        if (!chordGeom)
            continue;
        
        const Note* note = chordGeom->chord().firstNote();
        if (note->beams().empty())
            continue;
        
        auto& chords = chordsByVoice[note->voice()];
        chords.push_back(chordGeom);
        
        const auto& beam = note->beams().front();
        if (beam->type() == Beam::Type::Begin) {
        } else if (beam->type() == Beam::Type::Continue) {
        } else if (beam->type() == Beam::Type::End) {
            std::unique_ptr<BeamGeometry> beamGeom(new BeamGeometry(chords));
            ChordGeometry* firstChordGeom = chords.front();
            beamGeom->setLocation(firstChordGeom->location());
            chords.clear();
            
            _geometry->addGeometry(std::move(beamGeom));
        }
    }
}

void MeasureGeometryFactory::adjustBounds(MeasureGeometry* geometry) {
    const auto measureIndex = geometry->measure().index();
    auto newBounds = geometry->subGeometriesFrame();
    newBounds = join(newBounds, {Point{0, -1}, Size{0, 2 + _metrics.stavesHeight()}});
    newBounds.origin.x = 0;
    newBounds.size.width = _spans.width(measureIndex);
    
    geometry->setBounds(newBounds);
    geometry->setVerticalAnchorPointValues(0, -newBounds.origin.y);
}

} // namespace
