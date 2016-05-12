// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

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
