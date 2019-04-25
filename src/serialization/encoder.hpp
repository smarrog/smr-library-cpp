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
        Encoder(std::ostream& os, size_t flags)
            : _os(os)
            , _flags(flags) {}

        std::ostream& _os;
        size_t _flags;

        bool hasFlag(uint32_t flag) {
            return (_flags & flag) > 0;
        }
    };
}

