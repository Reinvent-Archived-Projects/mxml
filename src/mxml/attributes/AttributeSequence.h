// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once
#include <vector>
#include <mxml/dom/Note.h>
#include <mxml/dom/Types.h>


namespace mxml {

template <typename T>
class AttributeSequence {
public:
    struct TimeIndex {
        std::size_t measureIndex;
        dom::time_t time;

        TimeIndex() : measureIndex(), time() {}
        bool operator<(const TimeIndex& rhs) const;
    };

    struct LineIndex {
        std::size_t partIndex;
        int staff;
        int octave;
        dom::Pitch::Step step;

        LineIndex() : partIndex(), staff(1), octave(), step() {}
        bool operator==(const LineIndex& rhs) const;
    };
    
    struct Index {
        TimeIndex time;
        LineIndex line;
    };

public:
    /**
     Sort all items, call this after all attributes have been added or the results will be undefined.
     */
    void sort();

    static Index indexFromNote(const dom::Note& note);

protected:
    struct Item {
        Index index;
        T value;

        bool operator<(const Item& rhs) const {
            return index.time < rhs.index.time;
        }
    };

protected:
    std::vector<Item> _items;
};

} // namespace mxml

#include "AttributeSequence.hh"
