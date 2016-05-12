// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once
#include <type_traits>

namespace mxml {
namespace dom {

/**
 When using XML it is often necessary to know if a particular attribute or element was missing from the XML file. This
 class stores both a value and a flag to determine if the value was present in the source or if it is just a default or
 inherited value. This can also be achievied with only pointers but this approach has the advantage of not having to
 deal with ownership of the pointer and still being able to store a default value.
 */
template <typename T>
class Optional {
public:
    typedef T type;
    typedef T& type_ref;
    typedef const T& type_cref;
    
public:
    Optional() : _value(), _present(false) {}
    Optional(const T& value) : _value(value), _present(false) {}
    Optional(T&& value) : _value(value), _present(false) {}
    Optional(const T& value, bool present) : _value(value), _present(present) {}
    Optional(T&& value, bool present) : _value(value), _present(present) {}
    
    // Setters and getters
    const T& value() const {
        return _value;
    }
    T& value() {
        return _value;
    }
    
    bool isPresent() const {
        return _present;
    }
    void setPresent(bool present) {
        _present = present;
    }
    void setPresentValue(const T& value) {
        _value = value;
        _present = true;
    }
    void setPresentValue(T&& value) {
        _value = value;
        _present = true;
    }
    void setAbsentValue(const T& value) {
        _value = value;
        _present = false;
    }
    void setAbsentValue(T&& value) {
        _value = value;
        _present = false;
    }

    void reset() {
        _value = T();
        _present = false;
    }

    // Casting
    explicit operator bool() const {
        return _present;
    }
    operator type_ref() {
        return _value;
    }
    operator type_cref() const {
        return _value;
    }
    
private:
    T _value;
    bool _present;
};

// Factory methods
template <typename T>
static Optional<typename std::remove_reference<T>::type> presentOptional(const T& value) {
    return Optional<typename std::remove_reference<T>::type>(value, true);
}
template <typename T>
static Optional<typename std::remove_reference<T>::type> presentOptional(T&& value) {
    return Optional<typename std::remove_reference<T>::type>(value, true);
}
template <typename T>
static Optional<typename std::remove_reference<T>::type> absentOptional(const T& value) {
    return Optional<typename std::remove_reference<T>::type>(value, false);
}
template <typename T>
static Optional<typename std::remove_reference<T>::type> absentOptional(T&& value) {
    return Optional<typename std::remove_reference<T>::type>(value, false);
}
    
} // namespace dom
} // namespace mxml
