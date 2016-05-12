// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once
#include <mxml/Metrics.h>
#include <mxml/geometry/EndingGeometry.h>

#include <memory>
#include <vector>

namespace mxml {

class BarlineGeometry;
class MeasureGeometry;
    
class EndingGeometryFactory {
public:
    EndingGeometryFactory(const std::vector<MeasureGeometry*>& measureGeometries, const Metrics& metrics);
    std::vector<std::unique_ptr<EndingGeometry>> build();

private:
    void buildEndings(const MeasureGeometry& measureGeom, const BarlineGeometry& barlineGeom);

private:
    const std::vector<MeasureGeometry*>& _measureGeometries;
    const Metrics& _metrics;

    std::vector<std::unique_ptr<EndingGeometry>> _geometries;

    dom::Optional<dom::Ending> _startEnding;
    Point _startEndingLocation;
};

} // namespace mxml
