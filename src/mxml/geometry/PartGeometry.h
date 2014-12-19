//  Created by Alejandro Isaza on 2014-05-05.
//  Copyright (c) 2014 Venture Media Labs. All rights reserved.

#pragma once
#include "Geometry.h"
#include "MeasureGeometry.h"
#include "TieGeometry.h"

#include <mxml/dom/Part.h>
#include <mxml/dom/Direction.h>
#include <mxml/dom/Ornaments.h>

namespace mxml {

class BarlineGeometry;
class ChordGeometry;

class PartGeometry : public Geometry {
public:
    static const coord_t kStaffLineSpacing;
    static const std::size_t kStaffLineCount;
    
    /** Get the height of a single staff. */
    static coord_t staffHeight();
    
public:
    explicit PartGeometry(const dom::Part& part, const SpanCollection& spans);
    
    const dom::Part& part() const {
        return _part;
    }
    
    const std::vector<MeasureGeometry*> measureGeometries() const {
        return _measureGeometries;
    }
    const std::vector<TieGeometry*> tieGeometries() const {
        return _tieGeometries;
    }
    const std::vector<PlacementGeometry*> directionGeometries() const {
        return _directionGeometries;
    }
    
    /** Get the total height of all staves. This does not include padding above the top staff or below the bottom staff, therefore it is smaller than the geometry's height.
     */
    coord_t stavesHeight() const;
    
    /** Return the y position of the top line of a staff, relative to the top line of the topmost staff.
     */
    coord_t staffOrigin(int staffNumber) const;
    
    /** Return the y position of a note, relative to the top line of the topmost staff.
     */
    coord_t noteY(const dom::Attributes& attributes, const dom::Note& note) const;
    
private:
    void build();
    
    void buildDirections();
    void buildDirection(const MeasureGeometry&  measureGeom, const dom::Direction& direction);
    void buildWedge(const MeasureGeometry& measureGeom, const dom::Direction& direction);
    void buildWedge(const MeasureGeometry& startMeasureGeom, const dom::Direction& startDirection,
                    const MeasureGeometry& stopMeasureGeom, const dom::Direction& stopDirection);
    void buildPedal(const MeasureGeometry& measureGeom, const dom::Direction& direction);
    void buildPedal(const MeasureGeometry& startMeasureGeom, const dom::Direction& startDirection,
                    const MeasureGeometry& stopMeasureGeom, const dom::Direction& stopDirection);

    void buildOrnaments();
    void buildOrnaments(const MeasureGeometry& measureGeom, const ChordGeometry& chordGeom);
    void buildOrnament(const MeasureGeometry& measureGeom, const ChordGeometry& chordGeom, const dom::Ornaments& ornaments);

    void buildEndings();
    void buildEndings(const MeasureGeometry& measureGeom, const BarlineGeometry& barlineGeom);

    void buildLyrics();
    void buildLyrics(const MeasureGeometry& measureGeom, const ChordGeometry& chordGeom);
    void buildLyric(const MeasureGeometry& measureGeom, const ChordGeometry& chordGeom, const dom::Lyric& lyric);

    void placeDirection(PlacementGeometry& geometry);
    void swapPlacement(PlacementGeometry& geometry);

    void resolveCollisions();
    void resolveDirectionDirectionCollisions();
    void resolveDirectionCollision(std::vector<PlacementGeometry*>& geometries);
    
private:
    const dom::Part& _part;
    const SpanCollection& _spans;
    std::vector<std::pair<const MeasureGeometry*, const dom::Direction*>> _openSpanDirections;

    dom::Optional<dom::Ending> _startEnding;
    Point _startEndingLocation;
    
    std::vector<MeasureGeometry*> _measureGeometries;
    std::vector<TieGeometry*> _tieGeometries;
    std::vector<PlacementGeometry*> _directionGeometries;
};

} // namespace mxml
