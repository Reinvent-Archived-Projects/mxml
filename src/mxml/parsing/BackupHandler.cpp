//  Created by Alejandro Isaza on 2/25/2014.
//  Copyright (c) 2014 Venture Media Labs Inc. All rights reserved.

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
