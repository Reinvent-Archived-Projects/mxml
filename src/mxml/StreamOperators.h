//  Created by Alejandro Isaza on 2014-06-27.
//  Copyright (c) 2014 Venture Media Labs. All rights reserved.

#pragma once
#include <iostream>

#include <mxml/dom/Accidental.h>
#include <mxml/geometry/Point.h>
#include <mxml/geometry/Size.h>
#include <mxml/geometry/Rect.h>

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

inline std::ostream& operator<<(std::ostream& os, const mxml::dom::Accidental::Type& type) {
    return os << type.name;
}

} // namespace
