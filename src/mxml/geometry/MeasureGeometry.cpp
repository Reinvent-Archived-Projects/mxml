//  Created by Alejandro Isaza on 2014-04-28.
//  Copyright (c) 2014 Venture Media Labs. All rights reserved.

#include "MeasureGeometry.h"

#include "BarlineGeometry.h"
#include "BeamGeometry.h"
#include "ChordGeometry.h"
#include "ClefGeometry.h"
#include "KeyGeometry.h"
#include "NoteGeometry.h"
#include "PartGeometry.h"
#include "RestGeometry.h"
#include "TimeSignatureGeometry.h"

#include "../dom/Backup.h"
#include "../dom/Forward.h"
#include "../dom/Wedge.h"

namespace mxml {

using namespace dom;

const coord_t MeasureGeometry::kGraceNoteScale = 0.85;
const coord_t MeasureGeometry::kVerticalPadding = 40;

MeasureGeometry::MeasureGeometry(const Measure& measure, const PartGeometry& partGeometry, const SpanCollection& spans)
    : _measure(measure), _partGeometry(partGeometry), _spans(spans) {
    _measureIndex = partGeometry.part().indexOfMeasure(&measure);
    assert(_measureIndex != static_cast<std::size_t>(-1));
    build();
}

coord_t MeasureGeometry::staffY(const Attributes& attributes, const Note& note) {
    coord_t y = 20;
    if (note.pitch().isPresent()) {
        const Clef& clef = attributes.clef(note.staff());
        y = staffY(clef, note.pitch());
    } else if (note.rest().isPresent()) {
        const Clef& clef = attributes.clef(note.staff());
        y = staffY(clef, note.rest());
    } else if (note.defaultY().isPresent()) {
        y = note.defaultY();
    }
    return y * PartGeometry::kStaffLineSpacing / 10;
}

coord_t MeasureGeometry::staffY(const Clef& clef, const Pitch& pitch) {
    switch (clef.sign().value()) {
        case Clef::SIGN_G: return staffYInGClef(pitch.step(), pitch.octave());
        case Clef::SIGN_F: return staffYInFClef(pitch.step(), pitch.octave());
        case Clef::SIGN_C: return staffYInCClef(pitch.step(), pitch.octave());
        default: return 20;
    }
}

coord_t MeasureGeometry::staffY(const Clef& clef, const Rest& rest) {
    if (!rest.displayStep().isPresent() || !rest.displayOctave().isPresent())
        return 20;
    
    switch (clef.sign().value()) {
        case Clef::SIGN_G: return staffYInGClef(rest.displayStep(), rest.displayOctave());
        case Clef::SIGN_F: return staffYInFClef(rest.displayStep(), rest.displayOctave());
        case Clef::SIGN_C: return staffYInCClef(rest.displayStep(), rest.displayOctave());
        default: return 20;
    }
}

coord_t MeasureGeometry::staffYInGClef(Pitch::Step step, int octave) {
    coord_t y;
    switch (step) {
        case Pitch::STEP_C: y = 50; break;
        case Pitch::STEP_D: y = 45; break;
        case Pitch::STEP_E: y = 40; break;
        case Pitch::STEP_F: y = 35; break;
        case Pitch::STEP_G: y = 30; break;
        case Pitch::STEP_A: y = 25; break;
        case Pitch::STEP_B: y = 20; break;
        default: y = 20;
    }
    return y - (octave - 4) * 35;
}

coord_t MeasureGeometry::staffYInCClef(Pitch::Step step, int octave) {
    coord_t y;
    switch (step) {
        case Pitch::STEP_C: y = 20; break;
        case Pitch::STEP_D: y = 15; break;
        case Pitch::STEP_E: y = 10; break;
        case Pitch::STEP_F: y = 5; break;
        case Pitch::STEP_G: y = 0; break;
        case Pitch::STEP_A: y = -5; break;
        case Pitch::STEP_B: y = -10; break;
        default: y = 20;
    }
    return y - (octave - 5) * 35;
}

coord_t MeasureGeometry::staffYInFClef(Pitch::Step step, int octave) {
    coord_t y;
    switch (step) {
        case Pitch::STEP_C: y = 25; break;
        case Pitch::STEP_D: y = 20; break;
        case Pitch::STEP_E: y = 15; break;
        case Pitch::STEP_F: y = 10; break;
        case Pitch::STEP_G: y = 5; break;
        case Pitch::STEP_A: y = 0; break;
        case Pitch::STEP_B: y = -5; break;
        default: y = 20;
    }
    return y - (octave - 3) * 35;
}

void MeasureGeometry::build() {
    _currentTime = 0;
    _currentAttributes = _measure.baseAttributes();

    for (auto& node : _measure.nodes()) {
        if (const Attributes* attributes = dynamic_cast<const Attributes*>(node.get())) {
            _currentAttributes = *attributes;
            buildAttributes(attributes);
        } else if (const Barline* barline = dynamic_cast<const Barline*>(node.get())) {
            buildBarline(barline);
        } else if (const TimedNode* timedNode = dynamic_cast<const TimedNode*>(node.get())) {
            buildTimedNode(timedNode);
        }
    }

    std::vector<ChordGeometry*> chords;
    for (std::size_t i = 0; i < _geometries.size(); i += 1) {
        auto& geom = _geometries[i];
        ChordGeometry* chordGeom = dynamic_cast<ChordGeometry*>(geom.get());
        if (!chordGeom)
            continue;
        
        const Note* note = chordGeom->chord().firstNote();
        if (note->beams().empty())
            continue;
        
        chords.push_back(chordGeom);
        
        const Beam& beam = note->beams().front();
        if (beam.type() == Beam::TYPE_BEGIN) {
        } else if (beam.type() == Beam::TYPE_CONTINUE) {
        } else if (beam.type() == Beam::TYPE_END) {
            std::unique_ptr<BeamGeometry> beamGeom(new BeamGeometry(chords, _currentAttributes));
            ChordGeometry* firstChordGeom = chords.front();
            beamGeom->setLocation(firstChordGeom->location());
            chords.clear();
            
            addGeometry(std::move(beamGeom));
        }
    }

    setSize({_spans.width(_measureIndex), _partGeometry.stavesHeight() + 2*kVerticalPadding});
    setContentOffset({0, -kVerticalPadding});

    centerLoneRest();
}

void MeasureGeometry::buildAttributes(const Attributes* attributes) {
    for (int staff = 1; staff <= attributes->staves(); staff += 1) {
        if (!attributes->clef(staff).isPresent())
            continue;

        const Clef& clef = attributes->clef(staff);
        auto it = _spans.with(&clef);
        if  (it == _spans.end())
            continue;

        std::unique_ptr<ClefGeometry> geo(new ClefGeometry(clef));
        geo->setStaff(staff);

        const Span& span = *it;
        Point location;
        location.x = span.start() + span.width()/2 - _spans.origin(_measureIndex);
        location.y = _partGeometry.staffOrigin(staff) + _partGeometry.staffHeight()/2;
        geo->setLocation(location);
        
        addGeometry(std::move(geo));
    }
    
    for (int staff = 1; staff <= attributes->staves(); staff += 1) {
        if (!attributes->time().isPresent())
            continue;
        
        const Time& time = attributes->time();
        auto it = _spans.with(&time);
        if  (it == _spans.end())
            continue;
        const Span& span = *it;

        std::unique_ptr<TimeSignatureGeometry> geo(new TimeSignatureGeometry(time));
        geo->setStaff(staff);

        Point location;
        location.x = span.start() + span.width()/2 - _spans.origin(_measureIndex);
        location.y = _partGeometry.staffOrigin(staff) + _partGeometry.staffHeight()/2;
        geo->setLocation(location);
        
        addGeometry(std::move(geo));
    }
    
    for (int staff = 1; staff <= attributes->staves(); staff += 1) {
        if (!attributes->key(staff).isPresent())
            continue;
        
        const Key& key = attributes->key(staff);
        auto it = _spans.with(&key);
        if  (it == _spans.end())
            continue;
        const Span& span = *it;

        const Clef& clef = attributes->clef(staff);
        std::unique_ptr<KeyGeometry> geo(new KeyGeometry(key, clef));
        geo->setStaff(staff);

        Point location;
        location.x = span.start() + span.width()/2 - _spans.origin(_measureIndex);
        location.y = _partGeometry.staffOrigin(staff) + _partGeometry.staffHeight()/2;
        geo->setLocation(location);
        
        addGeometry(std::move(geo));
    }
}

void MeasureGeometry::buildBarline(const Barline* barline) {
    std::unique_ptr<BarlineGeometry> geo(new BarlineGeometry(*barline, _partGeometry));
    geo->setVerticalAnchorPointValues(0, 0);
    geo->setHorizontalAnchorPointValues(0, 0);

    auto it = _spans.with(barline);
    assert(it != _spans.end());

    const Span& span = *it;
    geo->setLocation({span.start() - _spans.origin(_measureIndex), 0});

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
    if (!chord->firstNote()->printObject())
        return;
    
    std::unique_ptr<ChordGeometry> geo(new ChordGeometry(*chord, _currentAttributes, _partGeometry));

    Point location = geo->refNoteLocation();
    if (chord->stem() == STEM_UP) {
        geo->setHorizontalAnchorPointValues(0, location.x - geo->contentOffset().x);
        geo->setVerticalAnchorPointValues(1, -(geo->size().height - (location.y - geo->contentOffset().y)));
    } else {
        geo->setHorizontalAnchorPointValues(1, -(geo->size().width - (location.x - geo->contentOffset().x)));
        geo->setVerticalAnchorPointValues(0, location.y - geo->contentOffset().y);
    }

    auto it = _spans.with(chord);
    assert(it != _spans.end());

    const Span& span = *it;
    if (chord->firstNote()->grace()) {
        location.x = span.start() - _spans.origin(_measureIndex) + geo->anchorPoint().x;
    } else {
        location.x = span.start() + span.eventOffset() - _spans.origin(_measureIndex);
    }
    geo->setLocation(location);

    addGeometry(std::move(geo));
}

void MeasureGeometry::buildRest(const Note* note) {
    assert (note->rest().isPresent());
    if (!note->printObject())
        return;
    
    std::unique_ptr<RestGeometry> geo(new RestGeometry(*note));

    auto it = _spans.with(note);
    assert(it != _spans.end());

    const Span& span = *it;
    Point location;
    location.x = span.start() + span.eventOffset() - _spans.origin(_measureIndex);
    location.y = _partGeometry.noteY(_currentAttributes, *note);
    geo->setLocation(location);
    addGeometry(std::move(geo));
}

void MeasureGeometry::centerLoneRest() {
    RestGeometry* onlyRest = nullptr;
    for (auto& geometry : geometries()) {
        NoteGeometry* note = dynamic_cast<NoteGeometry*>(geometry.get());
        if (note) {
            onlyRest = nullptr;
            break;
        }

        ChordGeometry* chord = dynamic_cast<ChordGeometry*>(geometry.get());
        if (chord) {
            onlyRest = nullptr;
            break;
        }

        RestGeometry* rest = dynamic_cast<RestGeometry*>(geometry.get());
        if (rest) {
            if (onlyRest) {
                onlyRest = nullptr;
                break;
            } else {
                onlyRest = rest;
            }
        }
    }
    if (!onlyRest)
        return;

    // If there is only a rest in the measure, it should be centered
    auto location = onlyRest->location();
    const Span& span = *_spans.with(&onlyRest->note());
    auto start = -_spans.origin(_measureIndex) + span.start();
    location.x = start + (size().width - start) / 2 - onlyRest->size().width/2;
    onlyRest->setLocation(location);
}

} // namespace mxml
