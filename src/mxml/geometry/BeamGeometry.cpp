// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "BeamGeometry.h"
#include "NoteGeometry.h"

namespace mxml {

const coord_t BeamGeometry::kMinStem = 5;
const coord_t BeamGeometry::kMaxSlope = 0.25;

const coord_t BeamGeometry::kStemLineWidth = 1;
const coord_t BeamGeometry::kBeamLineWidth = 4;
const coord_t BeamGeometry::kBeamLineSpacing = 2;
const coord_t BeamGeometry::kHookLength = 8;

BeamGeometry::BeamGeometry(std::vector<ChordGeometry*> chords)
: _chords(chords), _placement()
{
    build();
}

coord_t BeamGeometry::slope() const {
     return (_beamEnd.y - _beamBegin.y) / (_beamEnd.x - _beamBegin.x);
}

Point BeamGeometry::stemTip(const ChordGeometry* chordGeom) const {
    auto s = slope();
    
    auto stemDirection = chordGeom->stem()->stemDirection();

    Point point;
    if (stemDirection == dom::Stem::Up) {
        point.x = chordGeom->location().x + NoteGeometry::kQuarterWidth/2 - kStemLineWidth/2;
    } else {
        point.x = chordGeom->location().x - NoteGeometry::kQuarterWidth/2 + kStemLineWidth/2;
    }
    point.y = _beamEnd.y - s * (_beamEnd.x - point.x);

    return convertFromGeometry(point, chordGeom->parentGeometry());
}

void BeamGeometry::recomputeFrame() {
    bool first = true;
    Rect frame;
    for (ChordGeometry* chordGeom : _chords) {
        Rect geomFrame = chordGeom->frame();
        if (first)
            frame = geomFrame;
        else
            frame = join(frame, geomFrame);
        first = false;
    }
    frame.origin.y -= kBeamLineWidth/2;
    frame.size.height += kBeamLineWidth;
    
    setSize(frame.size);
    setHorizontalAnchorPointValues(0, location().x - frame.origin.x);

    const ChordGeometry* chordGeom = _chords.front();
    if (chordGeom->stem() && chordGeom->stem()->stemDirection() == dom::Stem::Up)
        setVerticalAnchorPointValues(1, -(frame.size.height - (location().y - frame.origin.y)));
    else
        setVerticalAnchorPointValues(0, location().y - frame.origin.y);
}

void BeamGeometry::build() {
    if (_chords.size() <= 1)
        return;

    const ChordGeometry* first = _chords.front();
    const ChordGeometry* last = _chords.back();

    auto stemDirection = dom::Stem::Up;
    if (first->stem())
        stemDirection = first->stem()->stemDirection();

    // Chose beam placement
    if (stemDirection == dom::Stem::Up)
        _placement = dom::Placement::Below;
    else
        _placement = dom::Placement::Above;

    Point firstLocation = first->convertToGeometry(first->refNoteLocation(), first->parentGeometry());
    Point lastLocation = last->convertToGeometry(last->refNoteLocation(), last->parentGeometry());

    // Set the location to the first chord's location
    setLocation(firstLocation);

    //find the minimum and maximum y values
    coord_t min_y = _location.y;
    coord_t max_y = _location.y;
    for (ChordGeometry* chordGeom : _chords) {
        chordGeom->setShowFlags(false);
        Point location = chordGeom->location();
        
        if (location.y < min_y)
            min_y = location.y;

        if (location.y > max_y)
            max_y = location.y;
    }

    if (stemDirection == dom::Stem::Up) {
        _beamBegin.x = firstLocation.x + NoteGeometry::kQuarterWidth/2 - kStemLineWidth;
        _beamBegin.y = first->frame().min().y;
    } else {
        _beamBegin.x = firstLocation.x - NoteGeometry::kQuarterWidth/2;
        _beamBegin.y = first->frame().max().y;
    }
    
    if (last->stem()->stemDirection() == dom::Stem::Up) {
        _beamEnd.x = lastLocation.x + NoteGeometry::kQuarterWidth/2;
        _beamEnd.y = last->frame().min().y;
    } else {
        _beamEnd.x = lastLocation.x - NoteGeometry::kQuarterWidth/2 + kStemLineWidth;
        _beamEnd.y = last->frame().max().y;
    }
    
    if (_beamEnd.x == _beamBegin.x)
        _beamEnd.x = _beamBegin.x + NoteGeometry::kQuarterWidth;
    
    // Cap slope
    Point midPoint = {(_beamEnd.x + _beamBegin.x)/2, (_beamEnd.y + _beamBegin.y)/2};
    coord_t s = slope();
    
    if (s > kMaxSlope) {
        s = kMaxSlope;
    } else if (s < -kMaxSlope) {
        s = -kMaxSlope;
    }
    _beamBegin.y = midPoint.y - s * (midPoint.x - _beamBegin.x);
    _beamEnd.y = midPoint.y - s * (midPoint.x - _beamEnd.x);
    
    // Move away so that stems are not too short
    coord_t dy = 0;
    for (ChordGeometry* chordGeom : _chords) {
        const dom::Chord& chord = chordGeom->chord();
        const coord_t beamsWidth = chord.firstNote()->beams().size() * (kBeamLineWidth + kBeamLineSpacing);

        if (chordGeom->stem()->stemDirection() == dom::Stem::Up) {
            coord_t beamx = chordGeom->frame().origin.x + chordGeom->size().width - kStemLineWidth/2;
            coord_t beamy = _beamEnd.y - s * (_beamEnd.x - beamx) - kBeamLineWidth/2;
            coord_t maxy = chordGeom->notesFrame().min().y - kMinStem - beamsWidth;
            if (beamy > maxy)
                dy = std::min(dy, maxy - beamy);
        } else {
            coord_t beamx = chordGeom->frame().origin.x + kStemLineWidth/2;
            coord_t beamy = _beamEnd.y - s * (_beamEnd.x - beamx) + kBeamLineWidth/2;
            coord_t miny = chordGeom->notesFrame().max().y + kMinStem + beamsWidth;
            if (beamy < miny)
                dy = std::max(dy, miny - beamy);
        }
    }
    _beamBegin.y += dy;
    _beamEnd.y += dy;
    
    // Resize chords
    for (ChordGeometry* chordGeom : _chords) {
        const dom::Chord& chord = chordGeom->chord();
        const coord_t beamsWidth = (chord.firstNote()->beams().size() - 1) * (kBeamLineWidth + kBeamLineSpacing);

        coord_t beamx;
        coord_t beamy = 0;
        if (chordGeom->stem()->stemDirection() == dom::Stem::Up) {
            beamx = chordGeom->location().x + NoteGeometry::kQuarterWidth/2 - kStemLineWidth/2;
            if (_placement == dom::Placement::Above)
                beamy = -beamsWidth;
        } else {
            beamx = chordGeom->location().x - NoteGeometry::kQuarterWidth/2 + kStemLineWidth/2;
            if (_placement == dom::Placement::Below)
                beamy = beamsWidth;
        }
        beamy += _beamEnd.y - s * (_beamEnd.x - beamx);

        chordGeom->extendStem(beamy);
    }
    
    recomputeFrame();
}

} // namespace mxml
