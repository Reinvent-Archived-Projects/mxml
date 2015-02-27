//  Copyright (c) 2015 Venture Media Labs. All rights reserved.

#include "ChordGeometryFactory.h"
#include <mxml/geometry/FermataGeometry.h>


namespace mxml {

ChordGeometryFactory::ChordGeometryFactory(const ScoreProperties& scoreProperties, const Metrics& metrics)
: _scoreProperties(scoreProperties),
  _metrics(metrics)
{
}

std::unique_ptr<ChordGeometry> ChordGeometryFactory::build(const dom::Chord& chord) {
    if (!chord.firstNote() || !chord.firstNote()->printObject)
        return {};

    _geometry.reset(new ChordGeometry{chord});

    Rect notesFrame = buildNotes(chord);
    buildAccidentals(notesFrame);
    buildNotations(chord, notesFrame);
    buildStem(chord);

    _geometry->setBounds(_geometry->subGeometriesFrame());

    return std::move(_geometry);
}

Rect ChordGeometryFactory::buildNotes(const dom::Chord& chord) {
    for (auto& note : chord.notes()) {
        std::unique_ptr<NoteGeometry> geom(new NoteGeometry(*note));
        Point loc{0, _metrics.noteY(*note)};
        geom->setLocation(loc);

        _geometry->_notes.push_back(geom.get());
        _geometry->addGeometry(std::move(geom));
    }

    // Sort notes by stem position
    dom::Stem stem = chord.stem();
    std::vector<NoteGeometry*> sorted = _geometry->_notes;
    std::sort(sorted.begin(), sorted.end(), [stem](const NoteGeometry* g1, const NoteGeometry* g2) {
        if (stem == dom::kStemUp)
            return g1->location().y < g2->location().y;
        else
            return g1->location().y > g2->location().y;
    });

    Rect notesFrame;
    for (std::size_t i = 0; i < sorted.size(); i += 1) {
        NoteGeometry* geom = sorted[i];
        Point loc = geom->location();

        // Notes that are too close need to be shifted sideways
        for (std::size_t j = i + 1; j < sorted.size(); j += 1) {
            NoteGeometry* noteGeom = sorted[j];
            if (loc.x == noteGeom->location().x && std::abs(loc.y - noteGeom->location().y) < Metrics::kStaffLineSpacing) {
                if (stem == dom::kStemUp)
                    loc.x += geom->size().width;
                else
                    loc.x -= geom->size().width;
            }
        }
        geom->setLocation(loc);
        _geometry->_refNoteLocation = loc;

        if (notesFrame.size.width == 0) {
            notesFrame = geom->frame();
        } else {
            notesFrame = join(notesFrame, geom->frame());
        }
    }

    // Build dots
    for (auto& noteGeometry : _geometry->_notes) {
        buildDot(*noteGeometry);
    }

    return notesFrame;
}

void ChordGeometryFactory::buildDot(const NoteGeometry& noteGeom) {
    const dom::Note& note = noteGeom.note();
    if (!note.dot())
        return;

    std::unique_ptr<DotGeometry> dotGeom(new DotGeometry);

    Point dotLocation = noteGeom.location();
    dotLocation.x += noteGeom.size().width/2 + dotGeom->anchorPoint().x + ChordGeometry::kDotSpacing;

    coord_t staffY = dotLocation.y - _metrics.staffOrigin(note.staff());
    if ((int)staffY % 10 == 0) {
        if (note.dot()->placement() == dom::kPlacementAbove)
            dotLocation.y -= 5;
        else
            dotLocation.y += 5;
    }
    dotGeom->setLocation(dotLocation);

    for (auto& noteGeometry : _geometry->_notes) {
        if (intersect(noteGeometry->frame(), dotGeom->frame()))
            dotLocation.x = noteGeometry->frame().max().x + dotGeom->anchorPoint().x + ChordGeometry::kDotSpacing;
    }
    dotGeom->setLocation(dotLocation);

    _geometry->addGeometry(std::move(dotGeom));
}

void ChordGeometryFactory::buildAccidentals(const Rect& notesFrame) {
    for (auto& geom : _geometry->_notes)
        buildAccidental(*geom, notesFrame);
}

void ChordGeometryFactory::buildAccidental(const NoteGeometry& noteGeom, const Rect& notesFrame) {
    const dom::Note& note = noteGeom.note();
    if (!note.pitch())
        return;

    int alter = note.alter();
    int previousAlter = _scoreProperties.alter(note);
    if (alter == previousAlter)
        return;

    std::unique_ptr<AccidentalGeometry> accGeom(new AccidentalGeometry(alter));

    Point accLocation = noteGeom.location();
    accLocation.x = notesFrame.origin.x - ChordGeometry::kAccidentalSpacing - accGeom->size().width/2;
    accGeom->setLocation(accLocation);

    _geometry->addGeometry(std::move(accGeom));
}

void ChordGeometryFactory::buildNotations(const dom::Chord& chord, const Rect& notesFrame) {
    if (chord.notes().empty())
        return;

    // Notations should be on the first note of the chord
    const dom::Note* note = chord.notes().front().get();

    if (!note || !note->notations())
        return;

    Rect frame = notesFrame;
    const auto& notations = note->notations();
    for (auto& artic : notations->articulations()) {
        buildArticulation(chord, *artic, frame);
    }

    if (notations->fermata()) {
        buildFermata(*notations->fermata(), frame);
    }
}

void ChordGeometryFactory::buildArticulation(const dom::Chord& chord, const dom::Articulation& articulation, Rect& notesFrame) {
    const int staff = chord.firstNote()->staff();
    std::unique_ptr<ArticulationGeometry> geom(new ArticulationGeometry(articulation, chord.stem()));

    bool above = true;
    if (articulation.placement().isPresent()) {
        above = articulation.placement() == dom::kPlacementAbove;
    } else if (_geometry->_stem) {
        above = _geometry->_stem->stemDirection() == dom::kStemDown;
    } else {
        above = chord.stem() == dom::kStemDown;
    }

    Size size = geom->size();
    coord_t staffY;
    Point location;
    location.x = notesFrame.center().x;
    if (above) {
        location.y = notesFrame.origin.y - size.height/2 - ChordGeometry::kArticulationSpacing;
        staffY = notesFrame.origin.y - _metrics.staffOrigin(staff);
    } else {
        location.y = notesFrame.max().y + size.height/2 + ChordGeometry::kArticulationSpacing;
        staffY = notesFrame.max().y - _metrics.staffOrigin(staff);
    }

    // Add extra spacing if there is a staff line between the note and the articulation
    if ((int)staffY % 10 == 0) {
        if (above)
            location.y -= ChordGeometry::kArticulationSpacing;
        else
            location.y += ChordGeometry::kArticulationSpacing;
    }

    // Avoid placing the on a staff line
    bool intersectsStaffLine = false;
    coord_t y = _metrics.staffOrigin(staff);
    for (int i = 1; i <= Metrics::kStaffLineCount; i += 1) {
        if (location.y - size.height/2 < y && location.y + size.height/2 > y) {
            intersectsStaffLine = true;
            break;
        }
        y += Metrics::kStaffLineSpacing;
    }
    if (intersectsStaffLine) {
        if (above)
            location.y = y - Metrics::kStaffLineSpacing/2;
        else
            location.y = y + Metrics::kStaffLineSpacing/2;
    }

    geom->setLocation(location);
    notesFrame = join(notesFrame, geom->frame());

    _geometry->addGeometry(std::move(geom));
}

void ChordGeometryFactory::buildFermata(const dom::Fermata& fermata, Rect& notesFrame) {
    std::unique_ptr<FermataGeometry> geom(new FermataGeometry(fermata));

    Size size = geom->size();
    Point location;
    location.x = _geometry->_refNoteLocation.x;

    bool above = fermata.type() == dom::Fermata::TYPE_UPRIGHT;
    if (above) {
        location.y = notesFrame.origin.y - size.height/2 - ChordGeometry::kFermataSpacing;
    } else {
        location.y = notesFrame.max().y + size.height/2 + ChordGeometry::kFermataSpacing;
    }

    geom->setLocation(location);
    notesFrame = join(notesFrame, geom->frame());

    _geometry->addGeometry(std::move(geom));
}

void ChordGeometryFactory::buildStem(const dom::Chord& chord) {
    const dom::Note* note = chord.firstNote();
    if (note->type() > dom::Note::TYPE_HALF || (note->stem() != dom::kStemUp && note->stem() != dom::kStemDown))
        return;

    bool flags = note->beams().empty() && chord.firstNote()->beams().empty();
    std::unique_ptr<StemGeometry> stem(new StemGeometry(*note, flags));
    _geometry->_stem = stem.get();
    _geometry->addGeometry(std::move(stem));
    
    // Find edge notes
    bool first = true;
    Point topLocation;
    Point bottomLocation;
    for (auto& note : _geometry->_notes) {
        if (first || note->location().y < topLocation.y)
            topLocation = note->location();
        if (first || note->location().y > bottomLocation.y)
            bottomLocation = note->location();
        first = false;
    }
    
    // Set the location of the stem
    Point stemLocation;
    if (chord.stem() == dom::kStemUp) {
        stemLocation = {_geometry->_refNoteLocation.x, topLocation.y};
    } else {
        stemLocation = {_geometry->_refNoteLocation.x, bottomLocation.y};
    }
    _geometry->_stem->setLocation(stemLocation);
    
    _geometry->_stem->setHorizontalAnchorPointValues(0, NoteGeometry::kQuarterWidth/2);
    if (chord.stem() == dom::kStemUp) {
        _geometry->_stem->setVerticalAnchorPointValues(1, -NoteGeometry::kHeight/2);
    } else {
        _geometry->_stem->setVerticalAnchorPointValues(0, NoteGeometry::kHeight/2);
    }
}

} // namespace mxml
