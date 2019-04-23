#pragma once

#include <map>
#include <functional>
#include <memory>

namespace smr {
    class Serializer;

    using SerializerConstructor = std::function<std::shared_ptr<Serializer>(uint32_t)>;

    class SerializerFactory final {
    public:
        SerializerFactory(const SerializerFactory&) = delete;
        SerializerFactory& operator = (SerializerFactory const&) = delete;
        SerializerFactory(SerializerFactory&&) noexcept = delete;
        SerializerFactory& operator = (SerializerFactory&&) noexcept = delete;

        static void reg(const std::string& type, SerializerConstructor constructor);
        static std::shared_ptr<Serializer> build(const std::string& type, uint32_t flags = 0);
    private:
        SerializerFactory() = default;

        static SerializerFactory& getInstance() {
            static SerializerFactory instance;
            return instance;
        }

        std::map<std::string, SerializerConstructor> _builders;
    };
}