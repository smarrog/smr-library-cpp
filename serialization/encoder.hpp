#pragma once

#include "serializationUtils.hpp"

#include <ostream>
#include <iomanip>

#define DECLARE_ENCODER(Serializer, Class)              \
    friend Serializer;                                  \
                                                        \
public:                                                 \
    Class(const Class&) = delete;                       \
    Class& operator = (Class const&) = delete;          \
    Class(Class&&) noexcept = delete;                   \
    Class& operator = (Class&&) noexcept = delete;      \
                                                        \
    void encode(const Serializable& value);             \
                                                        \
private:                                                \
    explicit Class(std::ostream& os, size_t flags)      \
        : Encoder(os, flags) {}

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

