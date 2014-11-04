//  Created by Alejandro Isaza on 2014-06-03.
//  Copyright (c) 2014 Venture Media Labs. All rights reserved.

#include "OrnamentsGeometry.h"

namespace mxml {

static const Size kMordentSize = {18, 13};
static const Size kInvertedMordentSize = {18, 7};
static const Size kTrillMarkSize = {18, 16};
static const Size kTurnSize = {24, 10};
static const Size kInvertedTurnSize = {24, 16};

OrnamentsGeometry::OrnamentsGeometry(const dom::Ornaments& ornaments) : _ornaments(ornaments) {
    if (ornaments.trillMark().isPresent()) {
        setSize(kTrillMarkSize);
    } else if (ornaments.turn().isPresent()) {
        setSize(kTurnSize);
    } else if (ornaments.invertedTurn().isPresent()) {
        setSize(kInvertedTurnSize);
    } else if (ornaments.mordent().isPresent()) {
        setSize(kMordentSize);
    } else if (ornaments.invertedMordent().isPresent()) {
        setSize(kInvertedMordentSize);
    }
}

} // namespace
