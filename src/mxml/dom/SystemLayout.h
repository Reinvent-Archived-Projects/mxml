// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once
#include "Optional.h"
#include "PageLayout.h"
#include "Position.h"
#include "Types.h"

#include <vector>

namespace mxml {
namespace dom {

/**
 System margins are relative to the page margins. Positive values indent and negative values reduce the margin size.
 */
struct SystemMargins {
    Optional<tenths_t> left;
    Optional<tenths_t> right;
};

/**
 Indicates a system divider (also known as system separation mark) between systems displayed on the same page. Dividers
 on the left and right side of the page are controlled by the left-divider and right-divider elements respectively. The
 default vertical position is half the system-distance value from the top of the system that is below the divider. The
 default horizontal position is the left and right system margin, respectively.

 When used in the print element, the system-dividers element affects the dividers that would appear between the current
 system and the previous system.
 */
struct SystemDivider {
    SystemDivider() : printObject(true), position(), horizontalAlignment(HAlign::Left), verticalAlignment(VAlign::Top) {}
    
    bool printObject;
    Position position;
    HAlign horizontalAlignment;
    VAlign verticalAlignment;
};

/**
 The system-dividers element indicates the presence or absence of system dividers (also known as system separation
 marks) between systems displayed on the same page. Dividers on the left and right side of the page are controlled by
 the left-divider and right-divider elements respectively. The default vertical position is half the system-distance
 value from the top of the system that is below the divider. The default horizontal position is the left and right
 system margin, respectively.

 When used in the print element, the system-dividers element affects the dividers that would appear between the current
 system and the previous system.
 */
struct SystemDividers {
    Optional<SystemDivider> leftDivider;
    Optional<SystemDivider> rightDivider;
};

/**
 A system is a group of staves that are read and played simultaneously. System layout includes left and right margins
 and the vertical distance from the previous system. The system distance is measured from the bottom line of the
 previous system to the top line of the current system. It is ignored for the first system on a page. The top system
 distance is measured from the page's top margin to the top line of the first system. It is ignored for all but the
 first system on a page.

 Sometimes the sum of measure widths in a system may not equal the system width specified by the layout elements due to
 roundoff or other errors. The behavior when reading MusicXML files in these cases is application-dependent. For
 instance, applications may find that the system layout data is more reliable than the sum of the measure widths, and
 adjust the measure widths accordingly.
 */
struct SystemLayout {
    SystemMargins systemMargins;
    Optional<tenths_t> systemDistance;
    Optional<tenths_t> topSystemDistance;
    SystemDividers systemDividers;
};

} // namespace dom
} // namespace mxml
