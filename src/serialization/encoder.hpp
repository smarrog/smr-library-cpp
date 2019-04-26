#pragma once

#include "serialization/types/serializable.hpp"

#include <ostream>
#include <iomanip>

namespace smr {
    class Encoder {
    public:
        Encoder(const Encoder&) = delete;
        Encoder& operator = (Encoder const&) = delete;
        Encoder(Encoder&&) noexcept = delete;
        Encoder& operator = (Encoder&&) noexcept = delete;

    protected:
        Encoder(std::ostream& os, const SerializerConfig& config)
            : _os(os)
            , _config(config) {}

        std::ostream& _os;
        const SerializerConfig& _config;
    };
}

