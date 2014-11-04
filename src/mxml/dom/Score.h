//  Created by Alejandro Isaza on 2/19/2014.
//  Copyright (c) 2014 Venture Media Labs Inc. All rights reserved.

#pragma once
#include "Credit.h"
#include "Defaults.h"
#include "Identification.h"
#include "Node.h"
#include "Part.h"

#include <memory>
#include <vector>

namespace mxml {
namespace dom {

class Score : public Node {
public:
    Score() : _parts() {}
    
    const Identification& identification() const {
        return _identification;
    }
    void setIdentification(const Identification& id) {
        _identification = id;
    }
    void setIdentification(Identification&& id) {
        _identification = id;
    }
    
    const Defaults& defaults() const {
        return _defaults;
    }
    void setDefaults(const Defaults& defaults) {
        _defaults = defaults;
    }
    void setDefaults(Defaults&& defaults) {
        _defaults = defaults;
    }
    
    const std::vector<Credit> credits() const {
        return _credits;
    }
    void addCredit(const Credit& credit) {
        _credits.push_back(credit);
    }
    void addCredit(Credit&& credit) {
        _credits.push_back(credit);
    }
    
    const std::vector<std::unique_ptr<Part>>& parts() const {
        return _parts;
    }
    void addPart(std::unique_ptr<Part>&& part) {
        _parts.push_back(std::move(part));
    }
    
private:
    Identification _identification;
    Defaults _defaults;
    std::vector<Credit> _credits;
    std::vector<std::unique_ptr<Part>> _parts;
};

} // namespace dom
} // namespace mxml
