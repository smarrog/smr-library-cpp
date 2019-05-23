#pragma once

#include "serialization/encoder.hpp"
#include "amf3Context.hpp"

namespace smr {
    class Amf3Encoder final : public Encoder {
    public:
        explicit Amf3Encoder(std::ostream& os, const SerializerConfig& config)
            : Encoder(os, config) {}

        Amf3Encoder(const Amf3Encoder&) = delete;
        Amf3Encoder& operator = (Amf3Encoder const&) = delete;
        Amf3Encoder(Amf3Encoder&&) noexcept = delete;
        Amf3Encoder& operator = (Amf3Encoder&&) noexcept = delete;

        void encode(const Serializable& serializable);


    private:
        Amf3Context _context;

        void encode(std::nullptr_t);
        void encode(Undefined);
        void encode(bool);
        void encode(int);
        void encode(double);
        void encode(const std::string&);
        void encode(const Array&);
        void encode(const Object&);

        void encodeHeadlessInteger(uint32_t value);
        void encodeHeadlessString(const std::string& value);
        void encodeDictionary(const Object&);
    };
}