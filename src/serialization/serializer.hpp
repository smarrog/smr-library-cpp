#pragma once

#include "serialization/config.hpp"
#include "serialization/flags.hpp"
#include "types/serializable.hpp"
#include "stuff/dummy.hpp"

namespace smr {
    class Serializer {
    public:
        explicit Serializer(SerializerConfig config)
            : _config(std::move(config)) {}

        Serializer(const Serializer&) = delete;
        Serializer& operator = (Serializer const&) = delete;
        Serializer(Serializer&&) noexcept = delete;
        Serializer& operator = (Serializer&&) noexcept = delete;

        virtual ~Serializer() = default;

        virtual Serializable decode(std::istream& is) = 0;
        virtual void encode(std::ostream& os, const Serializable& value) = 0;

        SerializerConfig& getConfig() {
            return _config;
        }

    private:
        SerializerConfig _config;
    };
}

#include "serialization/serializerFactory.hpp"

namespace smr {
    #define REGISTER_SERIALIZER(id, Class)                                          \
    const Dummy Class::_dummy = [](){                                               \
        SerializerFactory::reg(#id, [](SerializerConfig config) {                   \
            return std::shared_ptr<Serializer>(new Class(config));                  \
        });                                                                         \
        return Dummy();                                                             \
    }();

    #define DECLARE_SERIALIZER(Class)                                               \
    public:                                                                         \
        explicit Class(SerializerConfig config = SerializerConfig())                \
            : Serializer(std::move(config)) {}                                      \
                                                                                    \
        Class(const Class&) = delete;                                               \
        Class& operator = (Class const&) = delete;                                  \
        Class(Class&&) noexcept = delete;                                           \
        Class& operator = (Class&&) noexcept = delete;                              \
                                                                                    \
        ~Class() override = default;                                                \
                                                                                    \
        Serializable decode(std::istream& is) override;                             \
        void encode(std::ostream& os, const Serializable& value) override;          \
                                                                                    \
    private:                                                                        \
        static const Dummy _dummy;
}