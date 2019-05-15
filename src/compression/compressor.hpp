#pragma once

#include "compression/config.hpp"
#include "stuff/dummy.hpp"

#include <iostream>

namespace smr {
    class Compressor {

    public:
        explicit Compressor(CompressorConfig config)
            : _config(std::move(config)) {}

        Compressor(const Compressor&) = delete;
        Compressor& operator = (Compressor const&) = delete;
        Compressor(Compressor&&) noexcept = delete;
        Compressor& operator = (Compressor&&) noexcept = delete;

        virtual ~Compressor() = default;

        virtual void deflate(std::istream& is, std::ostream& os) = 0;
        virtual void inflate(std::istream& is, std::ostream& os) = 0;

        CompressorConfig& getConfig() {
            return _config;
        }

    private:
        CompressorConfig _config;
    };
}

#include "compressorFactory.hpp"

namespace smr {
    #define REGISTER_COMPRESSOR(id, Class)                              \
    const Dummy Class::_dummy = [](){                                   \
        CompressorFactory::reg(#id, [](CompressorConfig config) {       \
            return std::shared_ptr<Compressor>(new Class(config));      \
        });                                                             \
        return Dummy();                                                 \
    }();

    #define DECLARE_COMPRESSOR(Class)                                   \
    public:                                                             \
        explicit Class(CompressorConfig config = CompressorConfig())    \
            : Compressor(std::move(config)) {}                          \
                                                                        \
        Class(const Class&) = delete;                                   \
        Class& operator = (Class const&) = delete;                      \
        Class(Class&&) noexcept = delete;                               \
        Class& operator = (Class&&) noexcept = delete;                  \
                                                                        \
        ~Class() override = default;                                    \
                                                                        \
        void deflate(std::istream& is, std::ostream& os) override;      \
        void inflate(std::istream& is, std::ostream& os) override;      \
                                                                        \
    private:                                                            \
        static const Dummy _dummy;
}
