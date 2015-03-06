//  Copyright (c) 2014 Venture Media Labs. All rights reserved.

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
