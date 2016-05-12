// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once
#include "Optional.h"
#include "Types.h"

namespace mxml {
namespace dom {

/**
 Page margins are specified either for both even and odd pages, or via separate odd and even page number values. The
 type attribute is not needed when used as part of a print element. If omitted when the page-margins type is used in
 the defaults element, "both" is the default value.
 */
struct PageMargins {
    PageMargins() : type(MarginType::Both) {}

    Optional<tenths_t> left;
    Optional<tenths_t> right;
    Optional<tenths_t> top;
    Optional<tenths_t> bottom;

    enum class MarginType {
        Odd,
        Even,
        Both
    };
    MarginType type;
};

} // namespace dom
} // namespace mxml
