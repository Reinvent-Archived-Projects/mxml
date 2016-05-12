// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "PartHandler.h"
#include <cstring>

namespace mxml {
namespace parsing {

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

} // namespace parsing
} // namespace mxml
