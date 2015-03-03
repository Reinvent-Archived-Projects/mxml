//  Created by Alejandro Isaza on 2014-04-28.
//  Copyright (c) 2014 Venture Media Labs. All rights reserved.

#include "MeasureGeometry.h"

#include <iostream>

#include "BarlineGeometry.h"
#include "BeamGeometry.h"
#include "ChordGeometry.h"
#include "ClefGeometry.h"
#include "KeyGeometry.h"
#include "NoteGeometry.h"
#include "RestGeometry.h"
#include "TimeSignatureGeometry.h"
#include "factories/StemDirectionResolver.h"

#include <mxml/dom/Backup.h>
#include <mxml/dom/Forward.h>
#include <mxml/Metrics.h>

namespace mxml {

using namespace dom;

const coord_t MeasureGeometry::kGraceNoteScale = 0.85;

MeasureGeometry::MeasureGeometry(const Measure& measure,
                                 const SpanCollection& spans,
                                 const ScoreProperties& scoreProperties,
                                 const Metrics& metrics)
: _measure(measure),
  _spans(spans),
  _scoreProperties(scoreProperties),
  _metrics(metrics),
  _partIndex(metrics.partIndex()),
  _chordGeometryFactory(scoreProperties, metrics),
  _showNumber(false)
{
}

void MeasureGeometry::build(bool firstMeasureInSystem) {
    _currentTime = 0;
    
    if (firstMeasureInSystem)
        buildAttributes();

    if (firstMeasureInSystem || _scoreProperties.layoutType() == ScoreProperties::kLayoutTypeScroll)
        _showNumber = true;
    
    for (auto& node : _measure.nodes()) {
        if (const Attributes* attributes = dynamic_cast<const Attributes*>(node.get())) {
            buildAttributes(attributes);
        } else if (const Barline* barline = dynamic_cast<const Barline*>(node.get())) {
            buildBarline(barline);
        } else if (const TimedNode* timedNode = dynamic_cast<const TimedNode*>(node.get())) {
            buildTimedNode(timedNode);
        }
    }

    StemDirectionResolver resolver;
    resolver.resolve(this);
    
    std::vector<ChordGeometry*> chords;
    for (std::size_t i = 0; i < _geometries.size(); i += 1) {
        auto& geom = _geometries[i];
        ChordGeometry* chordGeom = dynamic_cast<ChordGeometry*>(geom.get());
        if (!chordGeom)
            continue;

        // Fix chord for new stem direction
        _chordGeometryFactory.resetForStem(chordGeom);

        const Note* note = chordGeom->chord().firstNote();
        if (note->beams().empty())
            continue;
        
        chords.push_back(chordGeom);
        
        const auto& beam = note->beams().front();
        if (beam->type() == Beam::kTypeBegin) {
        } else if (beam->type() == Beam::kTypeContinue) {
        } else if (beam->type() == Beam::kTypeEnd) {
            std::unique_ptr<BeamGeometry> beamGeom(new BeamGeometry(chords));
            ChordGeometry* firstChordGeom = chords.front();
            beamGeom->setLocation(firstChordGeom->location());
            chords.clear();
            
            addGeometry(std::move(beamGeom));
        }
    }

    adjustBounds();
    centerLoneRest();
}

void MeasureGeometry::buildAttributes() {
    const auto time = 0;
    for (int staff = 1; staff <= _metrics.staves(); staff += 1) {
        auto clef = _scoreProperties.clef(_partIndex, _measure.index(), staff, time);
        auto key = _scoreProperties.key(_partIndex, _measure.index(), staff, time);
        
        if (buildClefGeometry(clef, staff))
            builtGeometries.insert(clef);
        
        if (buildKeyGeometry(key, staff, time))
            builtGeometries.insert(key);
    }
}

void MeasureGeometry::buildAttributes(const Attributes* attributes) {
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

bool MeasureGeometry::buildClefGeometry(const dom::Clef* clef, int staff) {
    if (!clef)
        return false;
    
    auto it = _spans.with(clef, _measure.index());
    if (it == _spans.end())
        return false;
    
    std::unique_ptr<ClefGeometry> geo(new ClefGeometry(*clef));
    geo->setStaff(staff);
    
    const Span& span = *it;
    Point location;
    location.x = span.start() + span.width()/2 - _spans.origin(_measure.index());
    location.y = _metrics.staffOrigin(staff) + Metrics::staffHeight()/2;
    geo->setLocation(location);
    
    addGeometry(std::move(geo));
    
    return true;
}

bool MeasureGeometry::buildKeyGeometry(const dom::Key* key, int staff, dom::time_t time) {
    if (!key)
        return false;
    
    auto it = _spans.with(key, _measure.index());
    if  (it == _spans.end())
        return false;
    
    const Span& span = *it;
    std::unique_ptr<KeyGeometry> geo;
    
    auto clef = _scoreProperties.clef(_partIndex, _measure.index(), staff, time);
    if (key->fifths() == 0) {
        const auto& activeKey = _scoreProperties.key(_partIndex, _measure.index(), staff, time-1);
        if (!activeKey)
            return false;
        
        geo.reset(new KeyGeometry(*activeKey, *clef));
        geo->setNatural(true);
    } else {
        geo.reset(new KeyGeometry(*key, *clef));
    }
    
    Point location;
    location.x = span.start() + span.width()/2 - _spans.origin(_measure.index());
    location.y = _metrics.staffOrigin(staff) + Metrics::staffHeight()/2;
    geo->setLocation(location);
    geo->setStaff(staff);
    addGeometry(std::move(geo));
    
    return true;
}
    
bool MeasureGeometry::buildTimeGeometry(const dom::Time* time, int staff) {
    if (!time)
        return false;
    
    auto it = _spans.with(time, _measure.index());
    if  (it == _spans.end())
        return false;
    
    const Span& span = *it;
    
    std::unique_ptr<TimeSignatureGeometry> geo(new TimeSignatureGeometry(*time));
    geo->setStaff(staff);
    
    Point location;
    location.x = span.start() + span.width()/2 - _spans.origin(_measure.index());
    location.y = _metrics.staffOrigin(staff) + Metrics::staffHeight()/2;
    geo->setLocation(location);
    addGeometry(std::move(geo));
    
    return true;
}

void MeasureGeometry::buildBarline(const Barline* barline) {
    std::unique_ptr<BarlineGeometry> geo(new BarlineGeometry(*barline, _metrics));
    geo->setVerticalAnchorPointValues(0, 0);
    geo->setHorizontalAnchorPointValues(0, 0);

    auto it = _spans.with(barline);
    assert(it != _spans.end());

    const Span& span = *it;
    geo->setLocation({span.start() - _spans.origin(_measure.index()), 0});

    addGeometry(std::move(geo));
}

void MeasureGeometry::buildTimedNode(const TimedNode* node)  {
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

void MeasureGeometry::buildChord(const Chord* chord) {
    auto geo = _chordGeometryFactory.build(*chord);
    if (!geo)
        return;

    Point location = geo->refNoteLocation();

    // Assume that the stem is up, we'll redo this later
    geo->setHorizontalAnchorPointValues(0, location.x - geo->contentOffset().x);
    geo->setVerticalAnchorPointValues(1, -(geo->size().height - (location.y - geo->contentOffset().y)));

    auto it = _spans.with(chord);
    assert(it != _spans.end());

    const Span& span = *it;
    if (chord->firstNote()->grace()) {
        location.x = span.start() - _spans.origin(_measure.index()) + geo->anchorPoint().x;
    } else {
        location.x = span.start() + span.eventOffset() - _spans.origin(_measure.index());
    }
    geo->setLocation(location);

    addGeometry(std::move(geo));
}

void MeasureGeometry::buildRest(const Note* note) {
    assert (note->rest());
    if (!note->printObject)
        return;
    
    std::unique_ptr<RestGeometry> geo(new RestGeometry(*note));

    auto it = _spans.with(note);
    assert(it != _spans.end());

    const Span& span = *it;
    Point location;
    location.x = span.start() + span.eventOffset() - _spans.origin(_measure.index());
    location.y = _metrics.noteY(*note);
    geo->setLocation(location);
    addGeometry(std::move(geo));
}

void MeasureGeometry::centerLoneRest() {
    std::map<int, RestGeometry*> staffRests;
    
    for (auto& geometry : geometries()) {
        NoteGeometry* note = dynamic_cast<NoteGeometry*>(geometry.get());
        if (note) {
            staffRests[note->note().staff()-1] = nullptr;
            continue;
        }
        
        ChordGeometry* chord = dynamic_cast<ChordGeometry*>(geometry.get());
        if (chord) {
            staffRests[chord->chord().firstNote()->staff() - 1] = nullptr;
            continue;
        }
        
        RestGeometry* rest = dynamic_cast<RestGeometry*>(geometry.get());
        if (rest) {
            const int staff = rest->note().staff() - 1;
            if (staffRests.find(staff) != staffRests.end()) {
                staffRests[staff] = nullptr;
            } else {
                staffRests[staff] = rest;
            }
        }
    }
    
    // If there is only a Rest on this Measures Staff, center it
    for (auto& staffRest : staffRests) {
        RestGeometry* rest = staffRest.second;
        if (rest) {
            auto location = rest->location();
            const Span& span = *_spans.with(&rest->note());
            auto start = -_spans.origin(_measure.index()) + span.start();
            location.x = start + (size().width - start) / 2 - rest->size().width/2;
            rest->setLocation(location);
        }
    }
}

void MeasureGeometry::adjustBounds() {
    auto newBounds = subGeometriesFrame();
    newBounds = join(newBounds, {Point{0, -0.5}, Size{0, 1 + _metrics.stavesHeight()}});
    newBounds.origin.x = 0;
    newBounds.size.width = _spans.width(_measure.index());

    setBounds(newBounds);
    setVerticalAnchorPointValues(0, -newBounds.origin.y);
}

} // namespace mxml
