// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once
#include "Jump.h"

#include <mxml/dom/Score.h>

#include <map>
#include <string>
#include <vector>


namespace mxml {
namespace dom {
    class Direction;
}

class JumpFactory {
public:
    JumpFactory(const dom::Score& score);
    std::vector<Jump> build();

protected:
    void process(std::size_t measureIndex, const dom::Direction& direction);

private:
    const dom::Score& _score;
    std::map<std::string, Jump> _jumps;
};

}
