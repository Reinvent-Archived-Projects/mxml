// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "PrintHandler.h"
#include "TypeFactories.h"

#include <lxml/DoubleHandler.h>
#include <lxml/IntegerHandler.h>


namespace mxml {

using namespace parsing;

static const char* kNewSystemAttribute = "new-system";
static const char* kNewPageAttribute = "new-page";
static const char* kBlankPageAttribute = "blank-page";
static const char* kPageNumberAttribute = "page-number";

static const char* kPageLayoutTag = "page-layout";
static const char* kSystemLayoutTag = "system-layout";
static const char* kStaffLayoutTag = "staff-layout";
static const char* kMeasureLayoutTag = "measure-layout";
static const char* kMeasureNumberingTag = "measure-numbering";
static const char* kPartNameDisplayTag = "part-name-display";
static const char* kPartAbbreviationDisplayTag = "part-abbreviation-display";

void PrintHandler::startElement(const lxml::QName& qname, const AttributeMap& attributes) {
    _result.reset(new dom::Print{});

    AttributeMap::const_iterator it;

    it = attributes.find(kNewSystemAttribute);
    if (it != attributes.end())
        _result->newSystem = Factory::yesNoValue(it->second);

    it = attributes.find(kNewPageAttribute);
    if (it != attributes.end())
        _result->newPage = Factory::yesNoValue(it->second);

    it = attributes.find(kBlankPageAttribute);
    if (it != attributes.end())
        _result->blankPage = dom::presentOptional(lxml::IntegerHandler::parseInteger(it->second));

    it = attributes.find(kPageNumberAttribute);
    if (it != attributes.end())
        _result->pageNumber = dom::presentOptional(it->second);
}

lxml::RecursiveHandler* PrintHandler::startSubElement(const lxml::QName& qname) {
    if (strcmp(qname.localName(), kPageLayoutTag) == 0)
        return &_pageLayoutHandler;
    else if (strcmp(qname.localName(), kSystemLayoutTag) == 0)
        return &_systemLayoutHandler;
    else if (strcmp(qname.localName(), kStaffLayoutTag) == 0)
        return &_staffLayoutHandler;
    else if (strcmp(qname.localName(), kPartNameDisplayTag) == 0)
        return &_formattedTextHandler;
    else if (strcmp(qname.localName(), kPartAbbreviationDisplayTag) == 0)
        return &_formattedTextHandler;

    return &_genericNodeHandler;
}

void PrintHandler::endSubElement(const lxml::QName& qname, RecursiveHandler* parser) {
    if (strcmp(qname.localName(), kPageLayoutTag) == 0) {
        _result->pageLayout = dom::presentOptional(_pageLayoutHandler.result());
    } else if (strcmp(qname.localName(), kSystemLayoutTag) == 0) {
        _result->systemLayout = dom::presentOptional(_systemLayoutHandler.result());
    } else if (strcmp(qname.localName(), kStaffLayoutTag) == 0) {
        auto staffLayout = _staffLayoutHandler.result();
        _result->staffDistances[staffLayout.number] = staffLayout.staffDistance;
    } else if (strcmp(qname.localName(), kMeasureLayoutTag) == 0) {
        auto node = _genericNodeHandler.result();
        auto value = static_cast<dom::tenths_t>(lxml::DoubleHandler::parseDouble(node->text()));
        _result->measureDistance = dom::presentOptional(value);
    } else if (strcmp(qname.localName(), kMeasureNumberingTag) == 0) {
        // Not supported
    } else if (strcmp(qname.localName(), kPartNameDisplayTag) == 0) {
        auto partNameDisplayNode = _genericNodeHandler.result();
        auto displayTextNode = partNameDisplayNode->child("display-text");
        if (displayTextNode) {
            auto value = FormattedTextHandler::buildFromGenericNode(*displayTextNode);
            _result->partNameDisplay = dom::presentOptional(value);
        }
    } else if (strcmp(qname.localName(), kPartAbbreviationDisplayTag) == 0) {
        auto partNameDisplayNode = _genericNodeHandler.result();
        auto displayTextNode = partNameDisplayNode->child("display-text");
        if (displayTextNode) {
            auto value = FormattedTextHandler::buildFromGenericNode(*displayTextNode);
            _result->partAbbreviationDisplay = dom::presentOptional(value);
        }
    }
}

} // namespace mxml
