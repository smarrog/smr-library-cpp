#pragma once

#include <stdexcept>
#include <sstream>

namespace smr {
    class EncodeException : public std::runtime_error {
    public:
        EncodeException(const std::string& decoderType, const std::exception& exception)
            : std::runtime_error([&]() {
                return std::runtime_error(
                        std::string(decoderType + " encode error ->\n") +
                        exception.what());
            }()) {}
    };
}