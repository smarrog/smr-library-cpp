#pragma once

#include <stdexcept>

namespace smr {
    template <typename T>
    class TokenException : public std::runtime_error {
    public:
        explicit TokenException(T token)
            : std::runtime_error(std::string("Unexpected token ") + std::to_string(token)) {}

        TokenException(T token, const std::string& description)
            : std::runtime_error(std::string("Unexpected token ") + std::to_string(token) + description) {}
    };
}