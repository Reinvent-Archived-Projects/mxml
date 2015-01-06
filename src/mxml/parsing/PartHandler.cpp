//  Created by Alejandro Isaza on 2/20/2014.
//  Copyright (c) 2014 Venture Media Labs Inc. All rights reserved.

#include "PartHandler.h"
#include <cstring>

namespace mxml {

using dom::Part;
using lxml::QName;

static const char* kIdTag = "id";
static const char* kMeasureTag = "measure";

void PartHandler::startElement(const QName& qname, const AttributeMap& attributes) {
    _result.reset(new Part());
    _measureIndex = 0;

    auto id = attributes.find(kIdTag);
    if (id != attributes.end())
        _result->setId(id->second);
}

lxml::RecursiveHandler* PartHandler::startSubElement(const QName& qname) {
    if (strcmp(qname.localName(), kMeasureTag) == 0)
        return &_measureHandler;
    return 0;
}

void PartHandler::endSubElement(const QName& qname, RecursiveHandler* parser) {
    if (strcmp(qname.localName(), kMeasureTag) == 0) {
        auto measure = _measureHandler.result();
        measure->setIndex(_measureIndex++);
        measure->setParent(_result.get());
        _result->addMeasure(std::move(measure));
    }
}

} // namespace mxml
