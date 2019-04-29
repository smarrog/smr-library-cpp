#pragma once

#include "serialization/config.hpp"
#include "serialization/flags.hpp"
#include "serialization/types/serializable.hpp"

#include <string>
#include <stdexcept>
#include <istream>

namespace smr {
    class Decoder {
    public:
        Decoder(const Decoder&) = delete;
        Decoder& operator = (Decoder const&) = delete;
        Decoder(Decoder&&) noexcept = delete;
        Decoder& operator = (Decoder&&) noexcept = delete;

    protected:
        Decoder(std::istream& is, const SerializerConfig& config)
            : _is(is)
            , _config(config) {}

        std::istream& _is;
        const SerializerConfig& _config;
    };
}