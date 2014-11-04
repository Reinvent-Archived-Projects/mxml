//  Created by Alejandro Isaza on 2014-03-25.
//  Copyright (c) 2014 Venture Media Labs. All rights reserved.

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
    
    const std::vector<CreditWords>& creditWords() const {
        return _creditWords;
    }
    void addCreditWords(const CreditWords& words) {
        _creditWords.push_back(words);
    }
    
private:
    int _page;
    std::vector<CreditWords> _creditWords;
};

} // namespace dom
} // namespace mxml
