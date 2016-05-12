// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once
#include "Node.h"
#include "Position.h"


namespace mxml {
namespace dom {

/**
 The accidental type represents actual notated accidentals. Editorial and cautionary indications are indicated by
 attributes. Values for these attributes are "no" if not present. Specific graphic display such as parentheses,
 brackets, and size are controlled by the level-display attribute group.
 */
struct Accidental : public Node {
public:
    struct Type {
        const int alter;
        const char* name;

        static Type DoubleFlat;
        static Type Flat;
        static Type Natural;
        static Type Sharp;
        static Type DoubleSharp;

        static Type* byName(const char* name) noexcept;
        static Type* byAlter(int alter) noexcept;
        
        constexpr operator int() const { return alter; }

    private:
        constexpr Type(int alter, const char* name) noexcept : alter(alter), name(name) {}
    };
    
public:
    Accidental() : type(Type::Sharp) {}
    Accidental(Type type) : type(type) {}
    
public:
    Type type;
    Position poition;
};

} // namespace dom
} // namespace mxml
