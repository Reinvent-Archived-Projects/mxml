//  Created by Alejandro Isaza on 2014-04-24.
//  Copyright (c) 2014 Venture Media Labs. All rights reserved.

#pragma once

namespace mxml {
namespace dom {

enum Placement {
    PLACEMENT_ABOVE,
    PLACEMENT_BELOW
};

enum Stem {
    STEM_NONE,
    STEM_UP,
    STEM_DOWN,
    STEM_DOUBLE
};

enum StartStopContinue {
    TYPE_START,
    TYPE_CONTINUE,
    TYPE_STOP
};

enum Orientation {
    ORIENTATION_OVER,
    ORIENTATION_UNDER
};

typedef int time_t;

} // namespace dom
} // namespace mxml
