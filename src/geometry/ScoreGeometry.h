//  Created by Alejandro Isaza on 2014-06-26.
//  Copyright (c) 2014 Venture Media Labs. All rights reserved.

#pragma once
#include "Geometry.h"
#include "SpanCollection.h"
#include "dom/Score.h"

namespace mxml {

class ScoreGeometry : public Geometry {
public:
    ScoreGeometry(const dom::Score& score, bool naturalSpacing = true);

    const dom::Score& score() const {
        return _score;
    }
    const SpanCollection& spans() const {
        return _spans;
    }

private:
    const dom::Score& _score;
    SpanCollection _spans;
};

} // namespace
