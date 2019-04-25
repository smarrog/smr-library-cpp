#pragma once

#include <stdexcept>

namespace smr {
    template <typename T>
    class TokenException : public std::runtime_error {
    public:
        TokenException(T token)
            : std::runtime_error(std::string("Unexpected token >") + std::to_string(token) + std::string("<")) {}
    };
}