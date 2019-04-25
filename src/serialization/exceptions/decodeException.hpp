#pragma once

#include <stdexcept>
#include <sstream>

namespace smr {
    class DecodeException : public std::runtime_error {
    public:
        DecodeException(const std::string& decoderType, std::istream& is, const std::exception& exception, size_t nextBufSize = 50)
            : std::runtime_error([&]() {
                std::ostringstream oss;
                auto pos = is.tellg();
                while (!is.eof() && is.peek() != -1 && nextBufSize--) {
                    oss << static_cast<char>(is.get());
                }
                return std::runtime_error(
                        std::string(decoderType + " decode error ->\n") +
                        exception.what() + '\n' +
                        "position " + std::to_string(pos) + '\n' +
                        "next: " + oss.str());
            }()) {}
    };
}