#pragma once

#include "serialization/encoder.hpp"

namespace smr {
    class Base64Encoder final : public Encoder {
    public:
        explicit Base64Encoder(std::ostream& os, const SerializerConfig& config)
            : Encoder(os, config) {}

        Base64Encoder(const Base64Encoder&) = delete;
        Base64Encoder& operator = (Base64Encoder const&) = delete;
        Base64Encoder(Base64Encoder&&) noexcept = delete;
        Base64Encoder& operator = (Base64Encoder&&) noexcept = delete;

        void encode(const Serializable& serializable);
    private:
        using Buf3 = std::array<char, 3>;
        using Buf4 = std::array<char, 4>;

        Buf3 _buf3;
        Buf4 _buf4;
        size_t _buf3Index = 0;

        void flush();
    };
}