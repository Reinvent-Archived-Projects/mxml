//  Created by Alejandro Isaza on 2015-01-21.
//  Copyright (c) 2015 Venture Media Labs. All rights reserved.

#pragma once
#include <mxml/dom/Print.h>

namespace mxml {

class System {
public:
    System() = default;
    
public:
    const dom::Print* print;
    std::size_t measureBegin;
    std::size_t measureEnd;
};

} // namespace
