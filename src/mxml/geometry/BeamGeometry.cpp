//  Created by Alejandro Isaza on 2014-05-06.
//  Copyright (c) 2014 Venture Media Labs. All rights reserved.

#include "BeamGeometry.h"
#include "NoteGeometry.h"

namespace mxml {

const coord_t BeamGeometry::kMinStem = 5;
const coord_t BeamGeometry::kMaxSlope = 0.25;

const coord_t BeamGeometry::kStemLineWidth = 1;
const coord_t BeamGeometry::kBeamLineWidth = 4;
const coord_t BeamGeometry::kBeamLineSpacing = 2;
const coord_t BeamGeometry::kHookLength = 8;

BeamGeometry::BeamGeometry(std::vector<ChordGeometry*> chords, const dom::Attributes& attributes)
: _chords(chords), _attributes(attributes), _placement()
{
    build();
}

Point BeamGeometry::stemTip(const ChordGeometry* chordGeom) const {
    coord_t slope = (_beamEnd.y - _beamBegin.y) / (_beamEnd.x - _beamBegin.x);
    
    Point point;
    if (chordGeom->chord().stem() == dom::STEM_UP || chordGeom->chord().stem() == dom::STEM_DOUBLE) {
        point.x = chordGeom->location().x + NoteGeometry::kQuarterWidth/2 - kStemLineWidth/2;
    } else {
        point.x = chordGeom->location().x - NoteGeometry::kQuarterWidth/2 + kStemLineWidth/2;
    }
    point.y = _beamEnd.y - slope * (_beamEnd.x - point.x) - frame().origin.y;
    point.x -= frame().origin.x;
    
    return point;
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
    if (chordGeom->stem() && (chordGeom->stem()->note().stem() == dom::STEM_UP || chordGeom->stem()->note().stem() == dom::STEM_DOUBLE))
        setVerticalAnchorPointValues(1, -(frame.size.height - (location().y - frame.origin.y)));
    else
        setVerticalAnchorPointValues(0, location().y - frame.origin.y);
}

void BeamGeometry::build() {
    if (_chords.size() <= 1)
        return;

    const ChordGeometry* first = _chords.front();
    const ChordGeometry* last = _chords.back();

    // Chose beam placement
    if (first->stem() && (first->stem()->note().stem() == dom::STEM_UP || first->stem()->note().stem() == dom::STEM_DOUBLE))
        _placement = dom::PLACEMENT_BELOW;
    else
        _placement = dom::PLACEMENT_ABOVE;

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

    if (first->chord().stem() == dom::STEM_UP || first->chord().stem() == dom::STEM_DOUBLE) {
        _beamBegin.x = firstLocation.x + NoteGeometry::kQuarterWidth/2 - kStemLineWidth;
        _beamBegin.y = first->frame().min().y;
    } else {
        _beamBegin.x = firstLocation.x - NoteGeometry::kQuarterWidth/2;
        _beamBegin.y = first->frame().max().y;
    }
    
    if (last->chord().stem() == dom::STEM_UP || last->chord().stem() == dom::STEM_DOUBLE) {
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
    coord_t slope = (_beamEnd.y - _beamBegin.y) / (_beamEnd.x - _beamBegin.x);
    
    if (slope > kMaxSlope) {
        slope = kMaxSlope;
    } else if (slope < -kMaxSlope) {
        slope = -kMaxSlope;
    }
    _beamBegin.y = midPoint.y - slope * (midPoint.x - _beamBegin.x);
    _beamEnd.y = midPoint.y - slope * (midPoint.x - _beamEnd.x);
    
    // Move away so that stems are not too short
    coord_t dy = 0;
    for (ChordGeometry* chordGeom : _chords) {
        const dom::Chord& chord = chordGeom->chord();
        const coord_t beamsWidth = chord.firstNote()->beams().size() * (kBeamLineWidth + kBeamLineSpacing);

        if (chord.stem() == dom::STEM_UP || chord.stem() == dom::STEM_DOUBLE) {
            coord_t beamx = chordGeom->frame().origin.x + chordGeom->size().width - kStemLineWidth/2;
            coord_t beamy = _beamEnd.y - slope * (_beamEnd.x - beamx) - kBeamLineWidth/2;
            coord_t maxy = chordGeom->notesFrame().min().y - kMinStem - beamsWidth;
            if (beamy > maxy)
                dy = std::min(dy, maxy - beamy);
        } else {
            coord_t beamx = chordGeom->frame().origin.x + kStemLineWidth/2;
            coord_t beamy = _beamEnd.y - slope * (_beamEnd.x - beamx) + kBeamLineWidth/2;
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
        if (chord.stem() == dom::STEM_UP || chord.stem() == dom::STEM_DOUBLE) {
            beamx = chordGeom->location().x + NoteGeometry::kQuarterWidth/2 - kStemLineWidth/2;
            if (_placement == dom::PLACEMENT_ABOVE)
                beamy = -beamsWidth;
        } else {
            beamx = chordGeom->location().x - NoteGeometry::kQuarterWidth/2 + kStemLineWidth/2;
            if (_placement == dom::PLACEMENT_BELOW)
                beamy = beamsWidth;
        }
        beamy += _beamEnd.y - slope * (_beamEnd.x - beamx);

        chordGeom->extendStem(beamy);
    }
    
    recomputeFrame();
}

} // namespace mxml
