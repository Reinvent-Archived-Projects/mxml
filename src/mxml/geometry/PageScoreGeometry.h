// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once
#include "Geometry.h"
#include "SystemGeometry.h"

#include <mxml/ScoreProperties.h>
#include <mxml/dom/Score.h>

#include <vector>


namespace mxml {

class PageScoreGeometry : public Geometry {
public:
    PageScoreGeometry(const dom::Score& score, coord_t minWidth);

    const dom::Score& score() const {
        return _score;
    }
    const ScoreProperties& scoreProperties() const {
        return _scoreProperties;
    }
    const SpanCollection& spans() const {
        return *_spans;
    }
    const std::vector<SystemGeometry*>& systemGeometries() const {
        return _systemGeometries;
    }

    void setActiveRange(std::size_t startMeasureIndex, std::size_t endMeasureIndex);
    
protected:
    coord_t maxSystemWidth() const;
    coord_t maxSystemDistance() const;

    void setSystemDistances(const coord_t distance);

private:
    const dom::Score& _score;

    ScoreProperties _scoreProperties;
    std::unique_ptr<SpanCollection> _spans;
    std::vector<SystemGeometry*> _systemGeometries;
};

} // namespace
