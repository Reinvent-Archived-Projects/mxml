//  Created by Alejandro Isaza on 2014-04-24.
//  Copyright (c) 2014 Venture Media Labs. All rights reserved.

#pragma once

namespace mxml {
namespace dom {

enum Placement {
    kPlacementAbove,
    kPlacementBelow
};

enum Stem {
    kStemNone,
    kStemUp,
    kStemDown,
    kStemDouble
};

enum StartStopContinue {
    kStart,
    kContinue,
    kStop,
    kChange
};

enum Orientation {
    kOrientationOver,
    kOrientationUnder
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
enum HAlign {
    kHAlignLeft,
    kHalignCenter,
    kHAlignRight
};

typedef int time_t;

/**
 The valign type is used to indicate vertical alignment to the top, middle, bottom, or baseline of the text. Defaults
 are implementation-dependent.
 */
enum VAlign {
    kVAlignTop,
    kVAlignMiddle,
    kVAlignBottom,
    kVAlignBaseline
};

/**
 The justify attribute is used to indicate left, center, or right justification. The default value varies for different
 elements. For elements where the justify attribute is present but the halign attribute is not, the justify attribute
 indicates horizontal alignment as well as justification.
 */
enum Justify {
    kJustifyLeft,
    kJustifyCenter,
    kJustifyRight
};

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
