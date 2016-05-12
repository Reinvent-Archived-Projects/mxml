// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once
#include "CreditWords.h"
#include "Node.h"

#include <vector>

namespace mxml {
namespace dom {

class Credit : public Node {
public:
    Credit() : _page(1), _creditWords() {}
    
    int page() const {
        return _page;
    }
    void setPage(int page) {
        _page = page;
    }
    
    const std::vector<std::unique_ptr<CreditWords>>& creditWords() const {
        return _creditWords;
    }
    void addCreditWords(std::unique_ptr<CreditWords>&& words) {
        _creditWords.push_back(std::move(words));
    }
    
private:
    int _page;
    std::vector<std::unique_ptr<CreditWords>> _creditWords;
};

} // namespace dom
} // namespace mxml
