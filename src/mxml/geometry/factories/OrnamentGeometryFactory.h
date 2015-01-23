//  Created by Alejandro Isaza on 2015-01-22.
//  Copyright (c) 2015 Venture Media Labs. All rights reserved.

#pragma once
#include <mxml/Metrics.h>
#include <mxml/geometry/MeasureGeometry.h>
#include <mxml/geometry/OrnamentsGeometry.h>

#include <memory>
#include <vector>


namespace mxml {

class ChordGeometry;

class OrnamentGeometryFactory {
public:
    OrnamentGeometryFactory(const Geometry* parentGeometry, const std::vector<MeasureGeometry*>& measureGeometries, const Metrics& metrics);
    std::vector<std::unique_ptr<OrnamentsGeometry>> build();

private:
    void buildOrnaments(const MeasureGeometry& measureGeom, const ChordGeometry& chordGeom);
    void buildOrnament(const MeasureGeometry& measureGeom, const ChordGeometry& chordGeom, const dom::Ornaments& ornaments);
    void place(PlacementGeometry& geometry);

private:
    const Geometry* _parentGeometry;
    const std::vector<MeasureGeometry*>& _measureGeometries;
    const Metrics& _metrics;

    std::vector<std::unique_ptr<OrnamentsGeometry>> _geometries;
};

} // namespace mxml
