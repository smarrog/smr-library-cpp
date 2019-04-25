#pragma once

#include "serializationUtils.hpp"
#include "serializable.hpp"

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
    void encode(const Serializable& serializable) override {    \
        Encoder::encode(serializable);                          \
    }                                                           \
    void encode(nullptr_t value) override;                      \
    void encode(bool value) override;                           \
    void encode(int value) override;                            \
    void encode(double value) override;                         \
    void encode(const std::string& value) override;             \
    void encode(const Serializable::Array& value) override;     \
    void encode(const Serializable::Object& value) override;

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

        virtual void encode(const Serializable& serializable);

        virtual void encode(nullptr_t value) = 0;
        virtual void encode(bool value) = 0;
        virtual void encode(int value) = 0;
        virtual void encode(double value) = 0;
        virtual void encode(const std::string& value) = 0;
        virtual void encode(const Serializable::Array& value) = 0;
        virtual void encode(const Serializable::Object& value) = 0;

        bool hasFlag(uint32_t flag) {
            return (_flags & flag) > 0;
        }
    };
}

