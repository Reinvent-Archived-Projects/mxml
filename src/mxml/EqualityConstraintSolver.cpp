// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "EqualityConstraintSolver.h"

namespace mxml {

EqualityConstraintSolver::EqualityConstraintSolver() {

}

void EqualityConstraintSolver::setVariableCount(std::size_t count) {
    _variables.resize(count, {});
}

void EqualityConstraintSolver::addEqualConstraint(std::size_t a, std::size_t b) {
    auto& varA = _variables[a];
    varA.equal.insert(b);

    auto& varB = _variables[b];
    varB.equal.insert(a);
}

void EqualityConstraintSolver::addDifferentConstraint(std::size_t a, std::size_t b) {
    auto& varA = _variables[a];
    varA.different.insert(b);

    auto& varB = _variables[b];
    varB.different.insert(a);
}

std::size_t EqualityConstraintSolver::solve(std::function<void (const std::vector<bool>& values)> f) {
    _f = f;

    const auto size = variableCount();
    std::vector<Value> values(size, {});
    return solve(values, 0);
}

std::size_t EqualityConstraintSolver::solve(std::vector<Value>& values, std::size_t start) {
    if (start >= values.size())
        return 0;

    // Trivial case
    if (start == values.size() - 1) {
        if (values[start].isSet) {
            report(values);
            return 1;
        } else {
            values[start].isSet = true;
            values[start].value = false;
            report(values);
            values[start].value = true;
            report(values);
            return 2;
        }
    }

    auto& val = values[start];
    if (val.isSet)
        return solve(values, start + 1);

    std::size_t count = 0;
    val.isSet = true;
    {
        auto valuesCopy = values;
        valuesCopy[start].value = false;
        if (propagate(valuesCopy, start) && consistent(valuesCopy))
            count += solve(valuesCopy, start + 1);
    }
    {
        auto valuesCopy = values;
        valuesCopy[start].value = true;
        if (propagate(valuesCopy, start) && consistent(valuesCopy))
            count += solve(valuesCopy, start + 1);
    }

    return count;
}

bool EqualityConstraintSolver::propagate(std::vector<Value>& values, std::size_t i) {
    auto& soureVar = _variables[i];
    auto& sourceVal = values[i];

    for (auto j : soureVar.equal) {
        auto& targetVal = values[j];
        if (targetVal.isSet && targetVal.value != sourceVal.value) {
            // Dead end
            return false;
        }
        targetVal.isSet = true;
        targetVal.value = sourceVal.value;
    }

    for (auto j : soureVar.different) {
        auto& targetVal = values[j];
        if (targetVal.isSet && targetVal.value == sourceVal.value) {
            // Dead end
            return false;
        }
        targetVal.isSet = true;
        targetVal.value = !sourceVal.value;
    }

    return true;
}

bool EqualityConstraintSolver::consistent(const std::vector<Value>& values) const {
    const auto size = variableCount();
    for (std::size_t i = 0; i < size; i += 1) {
        auto& sourceVal = values[i];
        if (!sourceVal.isSet)
            continue;

        auto& sourceVar = _variables[i];
        for (auto j : sourceVar.equal) {
            auto& targetVal = values[j];
            if (!targetVal.isSet)
                continue;

            if (targetVal.value != sourceVal.value)
                return false;
        }
        for (auto j : sourceVar.different) {
            auto& targetVal = values[j];
            if (!targetVal.isSet)
                continue;

            if (targetVal.value == sourceVal.value)
                return false;
        }
    }

    return true;
}

void EqualityConstraintSolver::report(std::vector<Value>& values) {
    std::vector<bool> boolValues;
    for (auto& val : values)
        boolValues.push_back(val.value);
    _f(boolValues);
}

} // namespace mxml
