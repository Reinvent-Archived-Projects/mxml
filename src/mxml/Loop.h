// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once
#include <map>


namespace mxml {

class Loop {
public:
    Loop() : _range(),  _count(1) {}
    Loop(std::size_t begin, std::size_t end) : _range(begin, end), _count(1) {}

    std::size_t begin() const { return _range.begin; }
    std::size_t end() const { return _range.end; }
    std::size_t count() const { return _count; }

    bool isSkipped(std::size_t iteration, std::size_t measureIndex) const {
        auto it = _skips.equal_range(iteration);
        for (auto i = it.first; i != it.second; ++i) {
            auto& range = i->second;
            if (measureIndex >= range.begin && measureIndex < range.end)
                return true;
        }

        return false;
    }

    void setBegin(std::size_t begin) {
        _range.begin = begin;
    }
    void setEnd(std::size_t end) {
        _range.end = end;
    }
    void setCount(std::size_t count) {
        _count = count;
    }
    void skip(std::size_t iteration, std::size_t skipBegin, std::size_t skipEnd) {
        _skips.insert(std::make_pair(iteration, Range{skipBegin, skipEnd}));
    }

    bool operator==(const Loop& rhs) const {
        return _range == rhs._range;
    }

    bool operator<(const Loop& rhs) const {
        return _range < rhs._range;
    }

protected:
    struct Range {
        std::size_t begin;
        std::size_t end;

        Range() : begin(), end() {}
        Range(std::size_t begin, std::size_t end) : begin(begin), end(end) {}

        bool operator==(const Range& rhs) const {
            return begin == rhs.begin && end == rhs.end;
        }
        bool operator<(const Range& rhs) const {
            if (begin < rhs.begin)
                return true;
            if (begin > rhs.begin)
                return false;
            return end < rhs.end;
        }
    };

private:
    Range _range;
    std::multimap<std::size_t, Range> _skips;
    std::size_t _count;
};

}
