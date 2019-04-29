#pragma once

#include "serialization/decoder.hpp"

namespace smr {
    class IniDecoder final : public Decoder {
    public:
        explicit IniDecoder(std::istream& is, const SerializerConfig& config)
            : Decoder(is, config) {}
        IniDecoder(const IniDecoder&) = delete;
        IniDecoder& operator = (IniDecoder const&) = delete;
        IniDecoder(IniDecoder&&) noexcept = delete;
        IniDecoder& operator = (IniDecoder&&) noexcept = delete;

        Serializable decode();

    private:
        void decodeLine(std::string_view line);

        Object _sections;
        Object* _sectionPtr;
    };
}
