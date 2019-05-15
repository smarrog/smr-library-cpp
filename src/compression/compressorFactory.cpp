#include "compressorFactory.hpp"

#include <iostream>

using namespace smr;

void CompressorFactory::reg(const std::string& type, CompressorConstructor builder) noexcept {
    auto& factory = getInstance();
    if (factory._builders.find(type) != factory._builders.end()) {
        std::cerr << "Compressor factory internal error: attempt redefine compressor with type  (" << type << '"';
        exit(EXIT_FAILURE);
    }
    factory._builders.emplace(type, builder);
}

std::shared_ptr<Compressor> CompressorFactory::build(const std::string& type, CompressorConfig config) {
    auto& factory = getInstance();
    auto it = factory._builders.find(type);
    if (it == factory._builders.end()) {
        throw std::runtime_error("Serializer with type (" + type + ") was not defined");
    }
    return it->second(config);
}