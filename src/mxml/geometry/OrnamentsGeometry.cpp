// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "OrnamentsGeometry.h"

namespace mxml {

static const Size kMordentSize = {18, 13};
static const Size kInvertedMordentSize = {18, 7};
static const Size kTrillMarkSize = {18, 16};
static const Size kTurnSize = {24, 10};
static const Size kInvertedTurnSize = {24, 16};

OrnamentsGeometry::OrnamentsGeometry(const dom::Ornaments& ornaments, int staff)
: PlacementGeometry(dom::absentOptional(dom::Placement::Above), staff),
  _ornaments(ornaments)
{
    if (ornaments.placement())
        setPlacement(ornaments.placement()->placement());

    if (ornaments.trillMark()) {
        setSize(kTrillMarkSize);
    } else if (ornaments.turn()) {
        setSize(kTurnSize);
    } else if (ornaments.invertedTurn()) {
        setSize(kInvertedTurnSize);
    } else if (ornaments.mordent()) {
        setSize(kMordentSize);
    } else if (ornaments.invertedMordent()) {
        setSize(kInvertedMordentSize);
    }
}

} // namespace
