#pragma once

#include "types/serializable.hpp"
#include "serializerFactory.hpp"
#include "config.hpp"
#include "flags.hpp"
#include "stuff/dummy.hpp"

#include <string>
#include <vector>
#include <unordered_map>
#include <iostream>
#include <stdexcept>
#include <memory>
#include <sstream>

namespace smr {
    #define REGISTER_SERIALIZER(id, Class)                                          \
        const Dummy Class::_dummy = [](){                                           \
            SerializerFactory::reg(#id, [](SerializerConfig config) {               \
                return std::shared_ptr<Serializer>(new Class(config));              \
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
        explicit Class(SerializerConfig config)                                     \
            : Serializer(std::move(config)) {}                                      \
                                                                                    \
        static const Dummy _dummy;

    class Serializer {
    public:
        explicit Serializer(SerializerConfig config)
            : _config(std::move(config)) {}
        Serializer(const Serializer&) = delete;
        Serializer& operator = (Serializer const&) = delete;
        Serializer(Serializer&&) noexcept = delete;
        Serializer& operator = (Serializer&&) noexcept = delete;

        virtual ~Serializer() = default;

        virtual Serializable decode(std::istream& is) const = 0;
        virtual void encode(std::ostream& os, const Serializable& value) const = 0;

        SerializerConfig& getConfig() {
            return _config;
        }

    protected:
        SerializerConfig _config;
    };
}