//  Copyright (c) 2015 Venture Media Labs. All rights reserved.

#pragma once
#include <mxml/PageMetrics.h>
#include <mxml/ScoreProperties.h>
#include <mxml/SpanCollection.h>
#include <mxml/dom/Part.h>
#include <mxml/geometry/SystemGeometry.h>


namespace mxml {

class SystemGeometryFactory {
public:
    SystemGeometryFactory(const dom::Part& part, const ScoreProperties& scoreProperties, const PageMetrics& metrics, const SpanCollection& spans);
    ~SystemGeometryFactory();

    std::unique_ptr<SystemGeometry> build(const System& system);

private:
    const dom::Part& _part;
    const ScoreProperties& _scoreProperties;
    const PageMetrics& _metrics;
    const SpanCollection& _spans;

    std::unique_ptr<SystemGeometry> _systemGeometry;
};

} // namespace mxml
