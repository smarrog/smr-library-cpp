#pragma once

#include "undefined.hpp"
#include "array.hpp"
#include "object.hpp"

#include <string>
#include <iostream>
#include <variant>

namespace smr {
    class Serializable : public std::variant<
            Undefined,
            nullptr_t,
            bool,
            int,
            double,
            std::string,
            Array,
            Object
    > {
    public:
        using variant::variant;

        bool isUndefined() const {
            return std::holds_alternative<Undefined>(*this);
        }
        bool isNull() const {
            return std::holds_alternative<nullptr_t>(*this);
        }
        bool isBool() const {
            return std::holds_alternative<bool>(*this);
        }
        bool isInt() const {
            return std::holds_alternative<int>(*this);
        }
        bool isFloat() const {
            return std::holds_alternative<double>(*this);
        }
        bool isString() const {
            return std::holds_alternative<std::string>(*this);
        }
        bool isArray() const {
            return std::holds_alternative<Array>(*this);
        }
        bool isObject() const {
            return std::holds_alternative<Object>(*this);
        }

        nullptr_t asNull() const {
            return std::get<nullptr_t>(*this);
        }
        bool asBool() const {
            return std::get<bool>(*this);
        }
        int asInt() const {
            return std::get<int>(*this);
        }
        double asFloat() const {
        	if (isInt()) {
        		return static_cast<double>(asInt());
        	}
            return std::get<double>(*this);
        }
        const std::string& asString() const {
            return std::get<std::string>(*this);
        }
        const Array& asArray() const {
            return std::get<Array>(*this);
        }
        const Object& asObject() const {
            return std::get<Object>(*this);
        }

        Array& asArray() {
            return std::get<Array>(*this);
        }
        Object& asObject() {
            return std::get<Object>(*this);
        }
    };
}