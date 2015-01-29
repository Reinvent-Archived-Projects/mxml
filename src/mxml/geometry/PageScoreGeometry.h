//  Created by Alejandro Isaza on 2015-01-23.
//  Copyright (c) 2015 Venture Media Labs. All rights reserved.

#pragma once
#include "Geometry.h"
#include "SystemGeometry.h"

#include <mxml/ScoreProperties.h>
#include <mxml/dom/Score.h>

#include <vector>


namespace mxml {

class PageScoreGeometry : public Geometry {
public:
    PageScoreGeometry(const dom::Score& score, coord_t width);

    const dom::Score& score() const {
        return _score;
    }
    const ScoreProperties& scoreProperties() const {
        return _scoreProperties;
    }
    const std::vector<SystemGeometry*>& systemGeometries() const {
        return _systemGeometries;
    }

private:
    const dom::Score& _score;

    ScoreProperties _scoreProperties;
    std::vector<SystemGeometry*> _systemGeometries;
};

} // namespace
