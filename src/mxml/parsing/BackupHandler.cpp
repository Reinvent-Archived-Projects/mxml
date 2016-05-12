// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "BackupHandler.h"

namespace mxml {

using dom::Backup;
using lxml::QName;

static const char* kDurationTag = "duration";

void BackupHandler::startElement(const QName& qname, const AttributeMap& attributes) {
    _result.reset(new Backup());
}

lxml::RecursiveHandler* BackupHandler::startSubElement(const QName& qname) {
    if (strcmp(qname.localName(), kDurationTag) == 0)
        return &_integerHandler;
    return 0;
}

void BackupHandler::endSubElement(const QName& qname, RecursiveHandler* parser) {
    if (strcmp(qname.localName(), kDurationTag) == 0)
        _result->setDuration(_integerHandler.result());
}

} // namespace mxml
