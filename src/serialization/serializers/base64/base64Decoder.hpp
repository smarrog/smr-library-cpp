#pragma once

#include <sstream>
#include "serialization/decoder.hpp"

namespace smr {
    class Base64Decoder final : public Decoder {
    public:
        explicit Base64Decoder(std::istream& is, const SerializerConfig& config)
            : Decoder(is, config) {}

        Base64Decoder(const Base64Decoder&) = delete;
        Base64Decoder& operator=(Base64Decoder const&) = delete;
        Base64Decoder(Base64Decoder&&) noexcept = delete;
        Base64Decoder& operator=(Base64Decoder&&) noexcept = delete;

        Serializable decode();

    private:
        using Buf3 = std::array<char, 3>;
        using Buf4 = std::array<char, 4>;

        Buf3 _buf3;
        Buf4 _buf4;
        std::ostringstream _oss;
        size_t _buf4Index = 0;

        void flush();
    };
}