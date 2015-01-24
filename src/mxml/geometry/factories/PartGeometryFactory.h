//  Copyright (c) 2014 Venture Media Labs. All rights reserved.

#pragma once
#include <mxml/Metrics.h>
#include <mxml/ScoreProperties.h>
#include <mxml/SpanCollection.h>
#include <mxml/dom/Part.h>
#include <mxml/geometry/PartGeometry.h>


namespace mxml {

class PartGeometryFactory {
public:
    PartGeometryFactory(const dom::Part& part, const ScoreProperties& scoreProperties, const Metrics& metrics, const SpanCollection& spans);
    ~PartGeometryFactory();

    /**
     Build a part geometry with all the measures
     */
    std::unique_ptr<PartGeometry> build();

    /**
     Build a part geometry with only a subset of the measures.
     */
    std::unique_ptr<PartGeometry> build(std::size_t beginMeasure, std::size_t endMeasure);

private:
    const dom::Part& _part;
    const ScoreProperties& _scoreProperties;
    const Metrics& _metrics;
    const SpanCollection& _spans;

    std::unique_ptr<PartGeometry> _partGeometry;
};

} // namespace mxml
