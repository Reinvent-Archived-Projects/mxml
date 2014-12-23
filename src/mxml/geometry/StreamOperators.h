//  Created by Alejandro Isaza on 2014-06-27.
//  Copyright (c) 2014 Venture Media Labs. All rights reserved.

#pragma once
#include <iostream>

#include "Point.h"
#include "Size.h"
#include "Rect.h"

namespace mxml {

inline std::ostream& operator<<(std::ostream& os, const mxml::Point& point) {
    return os << "(" << point.x << ", " << point.y << ")";
}
inline std::ostream& operator<<(std::ostream& os, const mxml::Size& size) {
    return os << "[" << size.width << ", " << size.height << "]";
}
inline std::ostream& operator<<(std::ostream& os, const mxml::Rect& rect) {
    return os << "{" << rect.origin << ", " << rect.size << "}";
}

} // namespace
