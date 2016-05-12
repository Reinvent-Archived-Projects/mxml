// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "TimeModificationHandler.h"

namespace mxml {

static const char* kActualNotesTag = "actual-notes";
static const char* kNormalNotesTag = "normal-notes";

void TimeModificationHandler::startElement(const lxml::QName& qname, const AttributeMap& attributes) {
    _result.reset(new dom::TimeModification{});
}

lxml::RecursiveHandler* TimeModificationHandler::startSubElement(const lxml::QName& qname) {
    if (strcmp(qname.localName(), kActualNotesTag) == 0)
        return &_integerHandler;
    else if (strcmp(qname.localName(), kNormalNotesTag) == 0)
        return &_integerHandler;
    return 0;
}

void TimeModificationHandler::endSubElement(const lxml::QName& qname, RecursiveHandler* parser) {
    if (strcmp(qname.localName(), kActualNotesTag) == 0) {
        _result->actualNotes = _integerHandler.result();
    } else if (strcmp(qname.localName(), kNormalNotesTag) == 0) {
        _result->normalNotes = _integerHandler.result();
    }
}

} // namespace mxml
