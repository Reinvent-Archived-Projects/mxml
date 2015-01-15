//  Created by Alejandro Isaza on 2015-01-14.
//  Copyright (c) 2015 Venture Media Labs. All rights reserved.

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
