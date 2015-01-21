//  Copyright (c) 2015 Venture Media Labs. All rights reserved.

#pragma once
#include "Optional.h"
#include "PageMargins.h"
#include "Types.h"

namespace mxml {
namespace dom {

/**
 Page layout can be defined both in score-wide defaults and in the print element. Page margins are specified either for
 both even and odd pages, or via separate odd and even page number values. The type is not needed when used as part of
 a print element. If omitted when used in the defaults element, "both" is the default.
 */
struct PageLayout {
    Optional<tenths_t> pageHeight;
    Optional<tenths_t> pageWidth;

    PageMargins evenPageMargins;
    PageMargins oddPageMargins;
};

} // namespace dom
} // namespace mxml
