// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once
#include <functional>
#include <unordered_set>
#include <vector>


namespace mxml {

/**
 Given a number of positive or negative eqality constraints for boolean variables, EqualityConstraintSolver generates
 all possible valid variable assignments. An equality constraint is of the form `a=b` or `a!=b` for two variables `a`
 and `b`. For instance the set of constraints `a=b, b!=c` will generate the assignments {a,b,c} = {t,t,f}, {f,f,t}.
 */
class EqualityConstraintSolver {
public:
    struct Variable {
        std::unordered_set<std::size_t> equal;
        std::unordered_set<std::size_t> different;
    };

public:
    EqualityConstraintSolver();

    /**
     Set the number of variables in the system. Set this before adding any constraints.
     */
    void setVariableCount(std::size_t count);

    /**
     Get the number of variables in the system.
     */
    std::size_t variableCount() const {
        return _variables.size();
    }

    /**
     Add a positive equality constraint between two variables.

     @param a First variable index, 0 <= a < variableCount()
     @param b Second variable index, 0 <= a < variableCount()
     */
    void addEqualConstraint(std::size_t a, std::size_t b);

    /**
     Add a negative equality constraint between two variables.

     @param a First variable index, 0 <= a < variableCount()
     @param b Second variable index, 0 <= a < variableCount()
     */
    void addDifferentConstraint(std::size_t a, std::size_t b);

    /**
     Solve the system. The given function will be called with each solution that satisfies the constraints. The
     function will not be called if there are no solutions.

     @param f The function to call for each solution found.
     @return The number of solutions found.
     */
    std::size_t solve(std::function<void (const std::vector<bool>& values)> f);

protected:
    struct Value {
        bool isSet;
        bool value;
        Value() : isSet(false), value(false) {}
    };

    /**
     Solve a subsystem given the current assignment. The subsystem consists of all variables from `start` to the end.
     All variables before `start` need to be assigned and consistent.

     @param values The current value assignment. All values before `start` need to be set.
     @param start  The first variable in the subsystem.

     @return The number of solutions found in the subsystem.
     */
    std::size_t solve(std::vector<Value>& values, std::size_t start);

    /**
     Propagate the value for variable `i` to other variables.

     @param values The current value assignment.
     @param i      The variable index to propagate.

     @return `true` if the propagation didn't cause any conflicts, `false` otherwise.
     */
    bool propagate(std::vector<Value>& values, std::size_t i);

    /**
     Return true if set values are consistent with the system.
     */
    bool consistent(const std::vector<Value>& values) const;

    /**
     Report a solution by calling the user-provided function.
     */
    void report(std::vector<Value>& values);

private:
    std::vector<Variable> _variables;
    std::function<void (const std::vector<bool>& values)> _f;
};

} // namespace mxml
