#pragma once

#include <memory>
#include <functional>
#include <map>

#include "compression/config.hpp"

namespace smr {
    class Compressor;

    using CompressorConstructor = std::function<std::shared_ptr<Compressor>(CompressorConfig)>;

    class CompressorFactory final {
    public:
        CompressorFactory(const CompressorFactory&) = delete;
        CompressorFactory& operator = (CompressorFactory const&) = delete;
        CompressorFactory(CompressorFactory&&) noexcept = delete;
        CompressorFactory& operator = (CompressorFactory&&) noexcept = delete;

        static void reg(const std::string& type, CompressorConstructor) noexcept;
        static std::shared_ptr<Compressor> build(const std::string& type, CompressorConfig config = CompressorConfig());

    private:
        CompressorFactory() = default;

        static CompressorFactory& getInstance() {
            static CompressorFactory instance;
            return instance;
        }

        std::map<std::string, CompressorConstructor> _builders;
    };
}
