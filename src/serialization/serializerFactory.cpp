#include "serializerFactory.hpp"

#include <stdexcept>
#include <iostream>

using namespace smr;

void SerializerFactory::reg(const std::string& type, SerializerConstructor builder) {
    auto& factory = getInstance();
    if (factory._builders.find(type) != factory._builders.end()) {
        std::cerr << "Serializer factory internal error: attempt redefine serializer with type  (" << type << '"';
        exit(EXIT_FAILURE);
    }
    factory._builders.emplace(type, builder);
}

std::shared_ptr<Serializer> SerializerFactory::build(const std::string& type, SerializerConfig config) {
    auto& factory = getInstance();
    auto it = factory._builders.find(type);
    if (it == factory._builders.end()) {
        throw std::runtime_error("Serializer with type (" + type + ") was not defined");
    }
    return it->second(config);
}
