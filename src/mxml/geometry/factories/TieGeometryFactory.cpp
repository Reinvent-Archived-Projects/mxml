//  Created by Alejandro Isaza on 2014-05-05.
//  Copyright (c) 2014 Venture Media Labs. All rights reserved.

#include "TieGeometryFactory.h"
#include "ChordGeometry.h"
#include "MeasureGeometry.h"
#include "NoteGeometry.h"

#include <mxml/Metrics.h>

namespace mxml {

const coord_t TieGeometryFactory::kTieSpacing = 2;

TieGeometryFactory::TieGeometryFactory(const Geometry& parentGeometry, const Metrics& metrics)
: _parentGeometry(parentGeometry),
  _metrics(metrics)
{
}

std::vector<std::unique_ptr<TieGeometry>>&& TieGeometryFactory::buildTieGeometries(const std::vector<std::unique_ptr<Geometry>>& geometries) {
    _tieGeometries.clear();
    _tieStartGeometries.clear();
    _slurStartGeometries.clear();
    createGeometries(geometries);
    return std::move(_tieGeometries);
}

void TieGeometryFactory::createGeometries(const std::vector<std::unique_ptr<Geometry>>& geometries) {
    for (auto& geom : geometries) {
        if (auto measure = dynamic_cast<MeasureGeometry*>(geom.get())) {
            createGeometries(measure->geometries());
        } else if (auto chordGeom = dynamic_cast<ChordGeometry*>(geom.get())) {
            createGeometriesFromNotes(chordGeom->notes());
        } else if (auto noteGeom = dynamic_cast<NoteGeometry*>(geom.get())) {
            createGeometryFromNote(noteGeom);
        }
    }
}

void TieGeometryFactory::createGeometriesFromNotes(const std::vector<std::unique_ptr<NoteGeometry>>& notes) {
    for (auto& note : notes) {
        createGeometryFromNote(note.get());
    }
}

void TieGeometryFactory::createGeometriesFromNotes(const std::vector<NoteGeometry*>& notes) {
    for (auto& note : notes) {
        createGeometryFromNote(note);
    }
}

void TieGeometryFactory::createGeometryFromNote(NoteGeometry* noteGeometry) {
    const dom::Note& note = noteGeometry->note();
    if (!note.notations())
        return;
    
    const auto& notations = note.notations();
    
    for (auto& tie : notations->ties()) {
        auto key = std::make_pair(note.staff(), note.pitch().get());
        if (tie->type() == dom::kContinue) {
            _tieStartGeometries[key] = noteGeometry;
        } else if (tie->type() == dom::kStop) {
            auto startGeom = _tieStartGeometries.find(key);
            if (startGeom != _tieStartGeometries.end()) {
                std::unique_ptr<TieGeometry> tieGeom = buildTieGeometry(startGeom->second, noteGeometry, tie->placement());
                startGeom->second->setTieGeometry(tieGeom.get());
                noteGeometry->setTieGeometry(tieGeom.get());
                
                _tieGeometries.push_back(std::move(tieGeom));
                _tieStartGeometries.erase(startGeom);
            }
        }
    }
    
    for (auto& slur : notations->slurs()) {
        auto key = std::make_pair(note.staff(), slur->number());
        if (slur->type() == dom::kContinue) {
            _slurStartGeometries[key] = noteGeometry;
        } else if (slur->type() == dom::kStop) {
            auto startGeom = _slurStartGeometries.find(key);
            if (startGeom != _slurStartGeometries.end()) {
                std::unique_ptr<TieGeometry> slurGeom = buildSlurGeometry(startGeom->second, noteGeometry, slur->placement());
                _tieGeometries.push_back(std::move(slurGeom));
                startGeom->second->setTieGeometry(slurGeom.get());
                noteGeometry->setTieGeometry(slurGeom.get());
                
                _slurStartGeometries.erase(startGeom);
            }
        }
    }
}

std::unique_ptr<TieGeometry> TieGeometryFactory::buildTieGeometry(const NoteGeometry* start, const NoteGeometry* stop, const dom::Optional<dom::Placement>& placement) {
    std::unique_ptr<TieGeometry> tieGeom(new TieGeometry);
    
    Point startLocation;
    Point stopLocation;
    
    startLocation.x = start->frame().max().x;
    stopLocation.x = stop->frame().min().x;
    
    if (!placement.isPresent()) {
        coord_t startStaffY = startLocation.y - _metrics.staffOrigin(start->note().staff());
        coord_t stopStaffY = stopLocation.y - _metrics.staffOrigin(stop->note().staff());
        coord_t avgy = (startStaffY + stopStaffY) / 2;
        if (avgy < Metrics::staffHeight()/2)
            tieGeom->setPlacement(absentOptional(dom::kPlacementAbove));
        else
            tieGeom->setPlacement(absentOptional(dom::kPlacementBelow));
    }
    
    if (tieGeom->placement().value() == dom::kPlacementBelow) {
        startLocation.y = start->frame().max().y;
        stopLocation.y = stop->frame().max().y;
    } else {
        startLocation.y = start->frame().min().y;
        stopLocation.y = stop->frame().min().y;
    }
    
    tieGeom->setStartLocation(_parentGeometry.convertFromGeometry(startLocation, start->parentGeometry()));
    tieGeom->setStopLocation(_parentGeometry.convertFromGeometry(stopLocation, stop->parentGeometry()));
    
    return tieGeom;
}

std::unique_ptr<TieGeometry> TieGeometryFactory::buildSlurGeometry(const NoteGeometry* start, const NoteGeometry* stop, const dom::Optional<dom::Placement>& placement) {
    std::unique_ptr<TieGeometry> tieGeom(new TieGeometry);
    
    Point startLocation = start->location();
    Point stopLocation = stop->location();
    
    startLocation.x += kTieSpacing;
    stopLocation.x -= kTieSpacing;
    
    if (!placement.isPresent()) {
        coord_t startStaffY = startLocation.y - _metrics.staffOrigin(start->note().staff());
        coord_t stopStaffY = stopLocation.y - _metrics.staffOrigin(stop->note().staff());
        coord_t avgy = (startStaffY + stopStaffY) / 2;
        if (avgy < Metrics::staffHeight()/2)
            tieGeom->setPlacement(absentOptional(dom::kPlacementAbove));
        else
            tieGeom->setPlacement(absentOptional(dom::kPlacementBelow));
    }
    
    const ChordGeometry& startChordGeom = static_cast<const ChordGeometry&>(*start->parentGeometry());
    const ChordGeometry& stopChordGeom = static_cast<const ChordGeometry&>(*stop->parentGeometry());
    
    Rect startNotesFrame = startChordGeom.notesFrame();
    Rect stopNotesFrame = stopChordGeom.notesFrame();
    
    if (tieGeom->placement().value() == dom::kPlacementBelow) {
        startLocation.y = startNotesFrame.max().y + kTieSpacing;
        stopLocation.y = stopNotesFrame.max().y + kTieSpacing;
    } else {
        startLocation.y = startNotesFrame.min().y - kTieSpacing;
        stopLocation.y = stopNotesFrame.min().y - kTieSpacing;
    }
    
    // Avoid collisions with beamed sets
    if (!start->note().beams().empty() && start->note().beams().front()->type() != dom::Beam::kTypeEnd) {
        ChordGeometry* chordGeom = (ChordGeometry*)start->parentGeometry();
        Rect stemFrame = chordGeom->stem()->frame();
        
        if (tieGeom->placement().value() == dom::kPlacementBelow && start->note().stem() == dom::kStemDown) {
            startLocation.x = stemFrame.min().x + StemGeometry::kNoFlagWidth;
            startLocation.y = stemFrame.max().y + 2*kTieSpacing;
        } else if (tieGeom->placement().value() == dom::kPlacementAbove && start->note().stem() == dom::kStemUp) {
            startLocation.x = stemFrame.max().x - StemGeometry::kNoFlagWidth;
            startLocation.y = stemFrame.min().y - 2*kTieSpacing;
        }
    }
    if (!stop->note().beams().empty() && stop->note().beams().front()->type() != dom::Beam::kTypeBegin) {
        ChordGeometry* chordGeom = (ChordGeometry*)stop->parentGeometry();
        Rect stemFrame = chordGeom->stem()->frame();
        
        if (tieGeom->placement().value() == dom::kPlacementBelow && stop->note().stem() == dom::kStemDown) {
            stopLocation.x = stemFrame.min().x + StemGeometry::kNoFlagWidth;
            stopLocation.y = stemFrame.max().y + 2*kTieSpacing;
        } else if (tieGeom->placement().value() == dom::kPlacementAbove && stop->note().stem() == dom::kStemUp) {
            stopLocation.x = stemFrame.max().x - StemGeometry::kNoFlagWidth;
            stopLocation.y = stemFrame.min().y - 2*kTieSpacing;
        }
    }

    tieGeom->setStartLocation(_parentGeometry.convertFromGeometry(startLocation, start->parentGeometry()));
    tieGeom->setStopLocation(_parentGeometry.convertFromGeometry(stopLocation, stop->parentGeometry()));
    
    return tieGeom;
}

} // namespace mxml
