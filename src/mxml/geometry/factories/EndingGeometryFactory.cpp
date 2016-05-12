// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "EndingGeometryFactory.h"
#include <mxml/geometry/BarlineGeometry.h>
#include <mxml/geometry/MeasureGeometry.h>


namespace mxml {

EndingGeometryFactory::EndingGeometryFactory(const std::vector<MeasureGeometry*>& measureGeometries, const Metrics& metrics)
: _measureGeometries(measureGeometries),
  _metrics(metrics)
{}

std::vector<std::unique_ptr<EndingGeometry>> EndingGeometryFactory::build() {
    _geometries.clear();

    for (auto& measure: _measureGeometries) {
        for (auto& geom : measure->geometries()) {
            if (const BarlineGeometry* barline = dynamic_cast<const BarlineGeometry*>(geom.get()))
                buildEndings(*measure, *barline);
        }
    }

    return std::move(_geometries);
}

void EndingGeometryFactory::buildEndings(const MeasureGeometry& measureGeom, const BarlineGeometry& barlineGeom) {
    const dom::Barline& barline = barlineGeom.barline();
    if (!barline.ending())
        return;

    auto& ending = barline.ending();

    if ((ending->type() == dom::Ending::Type::Stop || ending->type() == dom::Ending::Type::Discontinue) && _startEnding.isPresent()) {
        Point stopLocation;
        stopLocation.x = measureGeom.frame().max().x - 1;
        stopLocation.y = measureGeom.origin().y - measureGeom.contentOffset().y - EndingGeometry::kHeight - 10;

        std::unique_ptr<EndingGeometry> endingGeom(new EndingGeometry(_startEnding, _startEndingLocation, *ending, stopLocation));
        endingGeom->setLocation(_startEndingLocation);
        _geometries.push_back(std::move(endingGeom));

        _startEnding.reset();
    } else if (ending->type() == dom::Ending::Type::Start) {
        _startEnding.setPresentValue(*ending);
        _startEndingLocation = measureGeom.location();
        _startEndingLocation.y = measureGeom.origin().y - measureGeom.contentOffset().y - EndingGeometry::kHeight - 10;
    }
}

} // namespace mxml
