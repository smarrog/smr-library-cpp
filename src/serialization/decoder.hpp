#pragma once

#include "serializationUtils.hpp"

#include <string>
#include <stdexcept>
#include <istream>

#define DECLARE_DECODER(Serializer, Class)              \
    friend Serializer;                                  \
                                                        \
public:                                                 \
    Class(const Class&) = delete;                       \
    Class& operator = (Class const&) = delete;          \
    Class(Class&&) noexcept = delete;                   \
    Class& operator = (Class&&) noexcept = delete;      \
                                                        \
    Serializable decode();                              \
                                                        \
private:                                                \
    explicit Class(std::istream& is, size_t flags)      \
        : Decoder(is, flags) {}

namespace smr {
    class Decoder {
    public:
        Decoder(const Decoder&) = delete;
        Decoder& operator = (Decoder const&) = delete;
        Decoder(Decoder&&) noexcept = delete;
        Decoder& operator = (Decoder&&) noexcept = delete;

    protected:
        Decoder(std::istream& is, size_t flags)
            : _is(is)
            , _flags(flags) {}

        std::istream& _is;
        size_t _flags;

        bool hasFlag(uint32_t flag) {
            return (_flags & flag) > 0;
        }
    };
}