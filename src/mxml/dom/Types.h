// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

namespace mxml {
namespace dom {

enum class Placement {
    Above,
    Below
};

enum class Stem {
    None,
    Up,
    Down,
    Double
};

enum StartStopContinue {
    kStart,
    kContinue,
    kStop,
    kChange
};

enum class Orientation {
    Over,
    Under
};

/**
 In cases where text extends over more than one line, horizontal alignment and justify values can be different. The
 most typical case is for credits, such as:

     Words and music by
       Pat Songwriter

 Typically this type of credit is aligned to the right, so that the position information refers to the right-most part
 of the text. But in this example, the text is center-justified, not right-justified.

 The halign attribute is used in these situations. If it is not present, its value is the same as for the justify
 attribute.
 */
enum class HAlign {
    Left,
    Center,
    Right
};

/**
 The valign type is used to indicate vertical alignment to the top, middle, bottom, or baseline of the text. Defaults
 are implementation-dependent.
 */
enum class VAlign {
    Top,
    Middle,
    Bottom,
    Baseline
};

/**
 The justify attribute is used to indicate left, center, or right justification. The default value varies for different
 elements. For elements where the justify attribute is present but the halign attribute is not, the justify attribute
 indicates horizontal alignment as well as justification.
 */
enum class Justify {
    Left,
    Center,
    Right
};

/**
 The time type is an integer representing the measure time in divisions.
 */
typedef int time_t;

/**
 The tenths type is a number representing tenths of interline staff space (positive or negative). Both integer and
 decimal values are allowed, such as 5 for a half space and 2.5 for a quarter space. Interline space is measured from
 the middle of a staff line.

 Distances in a MusicXML file are measured in tenths of staff space. Tenths are then scaled to millimeters within the
 scaling element, used in the defaults element at the start of a score. Individual staves can apply a scaling factor to
 adjust staff size. When a MusicXML element or attribute refers to tenths, it means the global tenths defined by the
 scaling element, not the local tenths as adjusted by the staff-size element.
 */
typedef float tenths_t;

/**
 The millimeters type is a number representing millimeters. This is used in the scaling element to provide a default
 scaling from tenths to physical units.
 */
typedef float millimeters_t;

} // namespace dom
} // namespace mxml
