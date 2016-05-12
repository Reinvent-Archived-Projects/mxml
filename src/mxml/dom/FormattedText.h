// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once
#include "Node.h"
#include "Optional.h"
#include "Position.h"

#include <string>


namespace mxml {
namespace dom {

/**
 Collects the common formatting attributes for text elements.
 */
struct FormattedText {
    FormattedText() : string(),
                      justify(Justify::Left),
                      horizontalAlignment(HAlign::Left),
                      verticalAlignment(VAlign::Top),
                      underline(false),
                      overline(false),
                      lineThrough(false)
    {}

    std::string string;

    Position position;

    Justify justify;
    HAlign horizontalAlignment;
    VAlign verticalAlignment;

    bool underline;
    bool overline;
    bool lineThrough;
};

} // namespace dom
} // namespace mxml
