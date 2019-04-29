#pragma once

#include "serialization/encoder.hpp"

namespace smr {
    class IniEncoder final : public Encoder {
    public:
        explicit IniEncoder(std::ostream& os, const SerializerConfig& config)
                : Encoder(os, config) {}

        IniEncoder(const IniEncoder&) = delete;
        IniEncoder& operator = (IniEncoder const&) = delete;
        IniEncoder(IniEncoder&&) noexcept = delete;
        IniEncoder& operator = (IniEncoder&&) noexcept = delete;

        void encode(const Serializable& serializable);
        void encodeSection(const std::string& name, const Object& data);
    };
}