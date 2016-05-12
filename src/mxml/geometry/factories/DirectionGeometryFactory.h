// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once
#include <mxml/geometry/PlacementGeometry.h>
#include <mxml/Metrics.h>

#include <memory>
#include <vector>


namespace mxml {

class BarlineGeometry;
class ChordGeometry;
class MeasureGeometry;

class DirectionGeometryFactory {
public:
    DirectionGeometryFactory();
    DirectionGeometryFactory(const Geometry* parentGeometry, const std::vector<MeasureGeometry*>& measureGeometries, const Metrics& metrics);

    // Reset the factory for a new parent geometry
    void reset(const Geometry* parentGeometry, const std::vector<MeasureGeometry*>& measureGeometries, const Metrics& metrics);

    std::vector<std::unique_ptr<PlacementGeometry>> build();

private:
    using MDPair = std::pair<const MeasureGeometry*, const dom::Direction*>;

    void buildDirection(const MeasureGeometry&  measureGeom, const dom::Direction& direction);
    void buildWedge(const MeasureGeometry& measureGeom, const dom::Direction& direction);
    void buildWedge(const MeasureGeometry& startMeasureGeom, const dom::Direction& startDirection,
                    const MeasureGeometry& stopMeasureGeom, const dom::Direction& stopDirection);

    void buildPedal(const MeasureGeometry& measureGeom, const dom::Direction& direction);
    MDPair pullPedalStart(const MeasureGeometry& stopMeasure, const dom::Direction& stopDirection);
    void buildPedal(const MeasureGeometry& startMeasureGeom, const dom::Direction& startDirection,
                    const MeasureGeometry& stopMeasureGeom, const dom::Direction& stopDirection);
    void buildPedalFromEdge(const dom::Direction& startDirection, const MeasureGeometry& stopMeasureGeom, const dom::Direction& stopDirection);
    void buildPedalToEdge(const MeasureGeometry& startMeasureGeom, const dom::Direction& startDirection);
    void buildPedalFromEdgeToEdge(const dom::Direction& startDirection);

    void buildOctaveShift(const MeasureGeometry& measureGeom, const dom::Direction& direction);
    MDPair pullOctaveShiftStart(const MeasureGeometry& stopMeasure, const dom::Direction& stopDirection);
    void buildOctaveShift(const MeasureGeometry& startMeasureGeom, const dom::Direction& startDirection,
                          const MeasureGeometry& stopMeasureGeom, const dom::Direction& stopDirection);
    void buildOctaveShiftFromEdge(const dom::Direction& startDirection, const MeasureGeometry& stopMeasureGeom, const dom::Direction& stopDirection);
    void buildOctaveShiftToEdge(const MeasureGeometry& startMeasureGeom, const dom::Direction& startDirection);
    void buildOctaveShiftFromEdgeToEdge(const dom::Direction& startDirection);

    void buildWords(const MeasureGeometry&  measureGeom, const dom::Direction& direction);
    void buildSegno(const MeasureGeometry&  measureGeom, const dom::Direction& direction);
    void buildCoda(const MeasureGeometry&  measureGeom, const dom::Direction& direction);

    Point spanOffsetInParentGeometry(const MeasureGeometry& measureGeometry, Point p);
    void placeDirection(PlacementGeometry& geometry);
    void swapPlacement(PlacementGeometry& geometry);

private:
    const Metrics* _metrics;
    const Geometry* _parentGeometry;
    std::vector<MeasureGeometry*> _measureGeometries;

    std::vector<std::unique_ptr<PlacementGeometry>> _geometries;
    std::vector<MDPair> _openSpanDirections;
    std::vector<const dom::Direction*> _previouslyOpenSpanDirections;
};

} // namespace mxml
