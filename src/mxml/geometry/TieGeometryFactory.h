//  Created by Alejandro Isaza on 2014-05-05.
//  Copyright (c) 2014 Venture Media Labs. All rights reserved.

#pragma once
#include "Geometry.h"
#include "NoteGeometry.h"
#include "TieGeometry.h"
#include "PartGeometry.h"

#include <mxml/dom/Note.h>

#include <map>
#include <memory>
#include <vector>

namespace mxml {

using std::map;
using std::pair;
using std::unique_ptr;
using std::vector;

class TieGeometryFactory {
public:
    static const coord_t kTieSpacing;
    
public:
    explicit TieGeometryFactory(const PartGeometry& partGeometry);
    
    vector<unique_ptr<TieGeometry>>&& buildTieGeometries(const vector<unique_ptr<Geometry>>& geometries);
    
private:
    void createGeometries(const vector<unique_ptr<Geometry>>& geometries);
    void createGeometriesFromNotes(const vector<unique_ptr<NoteGeometry>>& notes);
    void createGeometriesFromNotes(const vector<NoteGeometry*>& notes);
    void createGeometryFromNote(const NoteGeometry& noteGeometry);
    std::unique_ptr<TieGeometry> buildTieGeometry(const NoteGeometry* startGeom, const NoteGeometry* stopGeom, const dom::Optional<dom::Placement>& placement);
    std::unique_ptr<TieGeometry> buildSlurGeometry(const NoteGeometry* startGeom, const NoteGeometry* stopGeom, const dom::Optional<dom::Placement>& placement);
    
private:
    const PartGeometry& _partGeometry;

    vector<unique_ptr<TieGeometry>> _tieGeometries;
    map<std::pair<int, dom::Pitch>, const NoteGeometry*> _tieStartGeometries;
    map<std::pair<int, int>, const NoteGeometry*> _slurStartGeometries;
};

} // namespace mxml
