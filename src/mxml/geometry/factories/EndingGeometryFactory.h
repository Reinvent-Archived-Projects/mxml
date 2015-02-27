//  Created by Alejandro Isaza on 2015-01-22.
//  Copyright (c) 2015 Venture Media Labs. All rights reserved.

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
