// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once
#include "FormattedText.h"
#include "Node.h"
#include "Optional.h"
#include "PageLayout.h"
#include "SystemLayout.h"

#include <map>
#include <string>


namespace mxml {
namespace dom {

/**
 The print type contains general printing parameters, including the layout elements defined in the layout.mod file. The
 part-name-display and part-abbreviation-display elements used in the score.mod file may also be used here to change
 how a part name or abbreviation is displayed over the course of a piece. They take effect when the current measure or
 a succeeding measure starts a new system.

 Layout elements in a print statement only apply to the current page, system, staff, or measure. Music that follows
 continues to take the default values from the layout included in the defaults element.
 */
struct Print : public Node {
    Print() : newSystem(false), newPage(false) {}

    Optional<PageLayout> pageLayout;
    Optional<SystemLayout> systemLayout;
    std::map<int, tenths_t> staffDistances;

    /**
     The measure-distance element specifies the horizontal distance from the previous measure. This value is only used
     for systems where there is horizontal whitespace in the middle of a system, as in systems with codas. To specify
     the measure width, use the width attribute of the measure element.
     */
    Optional<tenths_t> measureDistance;

    Optional<FormattedText> partNameDisplay;
    Optional<FormattedText> partAbbreviationDisplay;

    /**
     The new-system attribute indicates whether to force a system break, or to force the current music onto the same
     system as the preceding music. Normally this is the first music data within a measure. If used in multi-part
     music, it should be placed in the same positions within each part, or the results are undefined.
     */
    bool newSystem;

    /**
     The new-page attribute indicates whether to force a page break, or to force the current music onto the same page
     as the preceding music. Normally this is the first music data within a measure. If used in multi-part music, it
     should be placed in the same positions within each part, or the results are undefined.
     */
    bool newPage;

    /**
     Version 2.0 adds a blank-page attribute. This is a positive integer value that specifies the number of blank pages
     to insert before the current measure. It is ignored if new-page is not "yes". These blank pages have no music, but
     may have text or images specified by the credit element. This is used to allow a combination of pages that are all
     text, or all text and images, together with pages of music.
     */
    Optional<int> blankPage;

    /**
     The page-number attribute sets the number of a new page; it is ignored if new-page is not "yes".
     */
    Optional<std::string> pageNumber;

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
