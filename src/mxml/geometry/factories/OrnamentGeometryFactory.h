// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once
#include <mxml/Metrics.h>
#include <mxml/geometry/OrnamentsGeometry.h>

#include <memory>
#include <vector>


namespace mxml {

class ChordGeometry;
class MeasureGeometry;

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
