#pragma once

#include "serialization/types/serializable.hpp"

#include <ostream>
#include <iomanip>

#define DECLARE_ENCODER(Serializer, Class)                      \
    friend Serializer;                                          \
                                                                \
public:                                                         \
    Class(const Class&) = delete;                               \
    Class& operator = (Class const&) = delete;                  \
    Class(Class&&) noexcept = delete;                           \
    Class& operator = (Class&&) noexcept = delete;              \
                                                                \
private:                                                        \
    explicit Class(std::ostream& os, size_t flags)              \
        : Encoder(os, flags) {}                                 \
                                                                \
protected:                                                      \
    void encode(const Serializable& serializable) override;

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

        virtual void encode(const Serializable& serializable) = 0;

        bool hasFlag(uint32_t flag) {
            return (_flags & flag) > 0;
        }
    };
}

