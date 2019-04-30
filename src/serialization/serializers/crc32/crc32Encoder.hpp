#pragma once

#include "serialization/encoder.hpp"

namespace smr {
    class Crc32Encoder final : public Encoder {
    public:
        explicit Crc32Encoder(std::ostream& os, const SerializerConfig& config)
            : Encoder(os, config) {}

        Crc32Encoder(const Crc32Encoder&) = delete;
        Crc32Encoder& operator=(Crc32Encoder const&) = delete;
        Crc32Encoder(Crc32Encoder&&) noexcept = delete;
        Crc32Encoder& operator=(Crc32Encoder&&) noexcept = delete;

        void encode(const Serializable& serializable);
    };
}