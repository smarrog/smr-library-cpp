#pragma once

#include <stdexcept>

namespace smr {
    class ImplementException : public std::runtime_error {
    public:
        ImplementException(const std::string what)
            : std::runtime_error(what + " is not implementeded") {}
    };
}