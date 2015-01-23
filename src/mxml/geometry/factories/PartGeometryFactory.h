//  Copyright (c) 2014 Venture Media Labs. All rights reserved.

#pragma once
#include <mxml/ScoreProperties.h>
#include <mxml/ScrollMetrics.h>
#include <mxml/SpanCollection.h>
#include <mxml/dom/Part.h>
#include <mxml/geometry/PartGeometry.h>


namespace mxml {

class PartGeometryFactory {
public:
    PartGeometryFactory(const dom::Part& part, const ScoreProperties& scoreProperties, const ScrollMetrics& metrics, const SpanCollection& spans);
    ~PartGeometryFactory();
    
    std::unique_ptr<PartGeometry> build();

private:
    const dom::Part& _part;
    const ScoreProperties& _scoreProperties;
    const ScrollMetrics& _metrics;
    const SpanCollection& _spans;

    std::unique_ptr<PartGeometry> _partGeometry;
};

} // namespace mxml
