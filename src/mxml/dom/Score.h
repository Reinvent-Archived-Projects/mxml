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
    
    const std::unique_ptr<Identification>& identification() const {
        return _identification;
    }
    void setIdentification(std::unique_ptr<Identification> id) {
        _identification = std::move(id);
    }
    
    const std::unique_ptr<Defaults>& defaults() const {
        return _defaults;
    }
    void setDefaults(std::unique_ptr<Defaults> defaults) {
        _defaults = std::move(defaults);
    }
    
    const std::vector<std::unique_ptr<Credit>>& credits() const {
        return _credits;
    }
    void addCredit(std::unique_ptr<Credit> credit) {
        _credits.push_back(std::move(credit));
    }
    
    const std::vector<std::unique_ptr<Part>>& parts() const {
        return _parts;
    }
    void addPart(std::unique_ptr<Part> part) {
        _parts.push_back(std::move(part));
    }
    
private:
    std::unique_ptr<Identification> _identification;
    std::unique_ptr<Defaults> _defaults;
    std::vector<std::unique_ptr<Credit>> _credits;
    std::vector<std::unique_ptr<Part>> _parts;
};

} // namespace dom
} // namespace mxml
