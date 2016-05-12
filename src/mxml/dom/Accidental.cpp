// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Accidental.h"
#include <cstring>


namespace mxml {
namespace dom {

Accidental::Type Accidental::Type::DoubleFlat{-2, "double-flat"};
Accidental::Type Accidental::Type::Flat{-1, "flat"};
Accidental::Type Accidental::Type::Natural{0, "natural"};
Accidental::Type Accidental::Type::Sharp{1, "sharp"};
Accidental::Type Accidental::Type::DoubleSharp{2, "double-sharp"};

Accidental::Type* Accidental::Type::byName(const char* name) noexcept {
    if (strcmp(name, DoubleFlat.name) == 0 || strcmp(name, "flat-flat") == 0)
        return &DoubleFlat;
    if (strcmp(name, Flat.name) == 0)
        return &Flat;
    if (strcmp(name, Natural.name) == 0)
        return &Natural;
    if (strcmp(name, Sharp.name) == 0)
        return &Sharp;
    if (strcmp(name, DoubleSharp.name) == 0 || strcmp(name, "sharp-sharp") == 0)
        return &DoubleSharp;
    return nullptr;
}

Accidental::Type* Accidental::Type::byAlter(int alter) noexcept {
    switch (alter) {
        case -2: return &DoubleFlat;
        case -1: return &Flat;
        case  0: return &Natural;
        case  1: return &Sharp;
        case  2: return &DoubleSharp;
        default: return nullptr;
    }
}

} // namespace dom
} // namespace mxml
