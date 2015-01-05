//  Created by Alejandro Isaza on 2014-05-14.
//  Copyright (c) 2014 Venture Media Labs. All rights reserved.

#pragma once
#include <lxml/BaseRecursiveHandler.h>

#include <mxml/dom/Sound.h>

namespace mxml {

class SoundHandler : public lxml::BaseRecursiveHandler<std::unique_ptr<dom::Sound>> {
public:
    void startElement(const lxml::QName& qname, const AttributeMap& attributes);
};

} // namespace mxml
