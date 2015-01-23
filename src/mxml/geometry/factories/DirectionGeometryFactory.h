//  Created by Alejandro Isaza on 2015-01-22.
//  Copyright (c) 2015 Venture Media Labs. All rights reserved.

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
    DirectionGeometryFactory(const Geometry* parentGeometry, const std::vector<MeasureGeometry*>& measureGeometries, const Metrics& metrics);
    std::vector<std::unique_ptr<PlacementGeometry>> build();

private:
    void buildDirection(const MeasureGeometry&  measureGeom, const dom::Direction& direction);
    void buildWedge(const MeasureGeometry& measureGeom, const dom::Direction& direction);
    void buildWedge(const MeasureGeometry& startMeasureGeom, const dom::Direction& startDirection,
                    const MeasureGeometry& stopMeasureGeom, const dom::Direction& stopDirection);
    void buildPedal(const MeasureGeometry& measureGeom, const dom::Direction& direction);
    void buildPedal(const MeasureGeometry& startMeasureGeom, const dom::Direction& startDirection,
                    const MeasureGeometry& stopMeasureGeom, const dom::Direction& stopDirection);
    void buildOctaveShift(const MeasureGeometry& measureGeom, const dom::Direction& direction);
    void buildOctaveShift(const MeasureGeometry& startMeasureGeom, const dom::Direction& startDirection,
                          const MeasureGeometry& stopMeasureGeom, const dom::Direction& stopDirection);

    void buildWords(const MeasureGeometry&  measureGeom, const dom::Direction& direction);
    void buildSegno(const MeasureGeometry&  measureGeom, const dom::Direction& direction);
    void buildCoda(const MeasureGeometry&  measureGeom, const dom::Direction& direction);

    void placeDirection(PlacementGeometry& geometry);
    void swapPlacement(PlacementGeometry& geometry);

private:
    const Geometry* _parentGeometry;
    const std::vector<MeasureGeometry*>& _measureGeometries;
    const Metrics& _metrics;

    std::vector<std::unique_ptr<PlacementGeometry>> _geometries;
    std::vector<std::pair<const MeasureGeometry*, const dom::Direction*>> _openSpanDirections;
};

} // namespace mxml
