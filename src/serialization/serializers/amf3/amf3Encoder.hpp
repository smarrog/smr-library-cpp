#pragma once

#include "serialization/encoder.hpp"

namespace smr {
    class Amf3Encoder final : public Encoder {
    public:
        explicit Amf3Encoder(std::ostream& os, const SerializerConfig& config)
            : Encoder(os, config) {}

        Amf3Encoder(const Amf3Encoder&) = delete;
        Amf3Encoder& operator = (Amf3Encoder const&) = delete;
        Amf3Encoder(Amf3Encoder&&) noexcept = delete;
        Amf3Encoder& operator = (Amf3Encoder&&) noexcept = delete;

        void encode(const Serializable& serializable);
    private:
        int encode29BitInt(int value);
    };
}