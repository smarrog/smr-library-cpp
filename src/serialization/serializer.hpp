#pragma once

#include "serialization/types/serializable.hpp"
#include "serializerFactory.hpp"
#include "../stuff/dummy.hpp"

#include <string>
#include <vector>
#include <unordered_map>
#include <iostream>
#include <stdexcept>
#include <memory>
#include <sstream>

#define REGISTER_SERIALIZER(id, Class)                                          \
    const Dummy Class::_dummy = [](){                                           \
        SerializerFactory::reg(#id, [](uint32_t flags) {                        \
            return std::shared_ptr<Serializer>(new Class(flags));               \
        });                                                                     \
        return Dummy();                                                         \
    }();

#define DECLARE_SERIALIZER(Class)                                               \
public:                                                                         \
    Class(const Class&) = delete;                                               \
    Class& operator = (Class const&) = delete;                                  \
    Class(Class&&) noexcept = delete;                                           \
    Class& operator = (Class&&) noexcept = delete;                              \
                                                                                \
    ~Class() override = default;                                                \
                                                                                \
    Serializable decode(std::istream& is) const override;                       \
    void encode(std::ostream& os, const Serializable& value) const override;    \
                                                                                \
private:                                                                        \
    explicit Class(uint32_t flags = 0)                                          \
        : Serializer(flags) {}                                                  \
                                                                                \
    static const Dummy _dummy;

namespace smr {
    class Serializer {
    public:
        static const uint32_t STRICT = 1u << 0;
        static const uint32_t PRETTY = 1u << 1;
        static const uint32_t ESCAPED_UNICODE = 1u << 2;

        explicit Serializer(uint32_t flags = 0)
            : _flags(flags) {}
        Serializer(const Serializer&) = delete;
        Serializer& operator = (Serializer const&) = delete;
        Serializer(Serializer&&) noexcept = delete;
        Serializer& operator = (Serializer&&) noexcept = delete;

        virtual ~Serializer() = default;

        virtual Serializable decode(std::istream& is) const = 0;
        virtual void encode(std::ostream& os, const Serializable& value) const = 0;

    protected:
        uint32_t _flags;
    };
}