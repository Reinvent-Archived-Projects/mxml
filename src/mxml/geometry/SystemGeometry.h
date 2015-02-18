//  Copyright (c) 2015 Venture Media Labs. All rights reserved.

#pragma once
#include <mxml/PageMetrics.h>
#include <mxml/ScoreProperties.h>
#include <mxml/dom/Part.h>

#include "Geometry.h"
#include "PartGeometry.h"


namespace mxml {

class SystemGeometry : public Geometry {
public:
    SystemGeometry(const dom::Score& score, const ScoreProperties& scoreProperties, const SpanCollection& spans, std::size_t systemIndex, coord_t width);

    const std::vector<PartGeometry*>& partGeometries() const {
        return _partGeometries;
    }
    const SpanCollection& spans() const {
        return _spans;
    }
    const PageMetrics& metrics(std::size_t partIndex) const {
        return *_metrics[partIndex];
    }
    const std::size_t systemIndex() const {
        return _systemIndex;
    }

    coord_t topPadding() const;
    coord_t bottomPadding() const;

    void setActiveRange(std::size_t startMeasureIndex, std::size_t endMeasureIndex);

private:
    const dom::Score& _score;
    const ScoreProperties& _scoreProperties;
    const SpanCollection& _spans;
    const std::size_t _systemIndex;

    std::vector<PartGeometry*> _partGeometries;
    std::vector<std::unique_ptr<PageMetrics>> _metrics;

    friend class SystemGeometryFactory;
};

} // namespace mxml
