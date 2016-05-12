// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once
#include "Node.h"
#include "Optional.h"
#include "PageLayout.h"
#include "Scaling.h"
#include "SystemLayout.h"

#include <map>


namespace mxml {
namespace dom {

/**
 Possible line types for the Default's lineWidths property.
 */
enum class LineType {
    Beam,
    Bracket,
    Dashes,
    Enclosure,
    Ending,
    Extend,
    HeavyBarline,
    Leger,
    LightBarline,
    OctaveShift,
    Pedal,
    SlurMiddle,
    SlurTip,
    Staff,
    Stem,
    TieMiddle,
    TieTip,
    TupletBracket,
    Wedge
};

/**
 Possible note types for the Default's noteSizes property.
 */
enum class NoteType {
    Cue,
    Grace,
    Large
};

/**
 Possible distance types for the Default's distances property.
 */
enum class DistanceType {
    Beam,
    Hyphen
};

/**
 The appearance type controls general graphical settings for the music's final form appearance on a printed page of
 display. This includes support for line widths, definitions for note sizes, and standard distances between notation
 elements, plus an extension element for other aspects of appearance.
 */
struct Appearance {
    /**
     The line-width type indicates the width of a line type in tenths.
     */
    std::map<LineType, tenths_t> lineWidths;

    /**
     The note-size type indicates the percentage of the regular note size to use for notes with a cue and large size as
     defined in the type element. A value of 100 would be identical to the size of a regular note as defined by the
     music font.
     */
    std::map<NoteType, float> noteSizes;

    /**
     The distance element represents standard distances between notation elements in tenths. The type attribute defines
     what type of distance is being defined.
     */
    std::map<DistanceType, tenths_t> distances;

    Optional<tenths_t> lineWidth(LineType type) const {
        auto it = lineWidths.find(type);
        if (it != lineWidths.end())
            return Optional<tenths_t>(it->second, true);
        return Optional<tenths_t>();
    }

    Optional<float> noteSize(NoteType type) const {
        auto it = noteSizes.find(type);
        if (it != noteSizes.end())
            return Optional<float>(it->second, true);
        return Optional<float>();
    }

    Optional<tenths_t> distance(DistanceType type) const {
        auto it = distances.find(type);
        if (it != distances.end())
            return Optional<tenths_t>(it->second, true);
        return Optional<tenths_t>();
    }
};

/**
 The defaults type specifies score-wide defaults for scaling, layout, and appearance.
 */
struct Defaults : public Node {
    Defaults() : scaling(),
                 pageLayout(),
                 systemLayout()
    {}
    
    Optional<Scaling> scaling;

    Optional<PageLayout> pageLayout;
    Optional<SystemLayout> systemLayout;
    std::map<int, tenths_t> staffDistances;

    Appearance appearance;

public:
    /**
     Get the staff layout for the given staff number.
     */
    Optional<tenths_t> staffDistance(int staffNumber = 2) const {
        auto it = staffDistances.find(staffNumber);
        if (it != staffDistances.end())
            return Optional<tenths_t>(it->second, true);
        return Optional<tenths_t>();
    }
};

} // namespace dom
} // namespace mxml
