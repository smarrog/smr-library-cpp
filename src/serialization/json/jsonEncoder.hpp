#pragma once

#include "jsonSerializer.hpp"
#include "../encoder.hpp"
#include "../serializable.hpp"

namespace smr {
    class JsonEncoder final : Encoder {
    DECLARE_ENCODER(JsonSerializer, JsonEncoder)

    private:
        size_t _offset = 0;

        inline void nextLine();
        inline void addSpacing();

        void encode(nullptr_t value);
        void encode(bool value);
        void encode(const std::string& value);
        void encode(const Serializable::Array& value);
        void encode(const Serializable::Object& value);

        template<typename T, typename std::enable_if<std::is_integral<T>::value || std::is_floating_point<T>::value, void>::type* = nullptr>
        void encode(T value);

        template <typename Container, typename ElemEncoder>
        void encodeContainer(const Container& container, char open, char close, ElemEncoder elemEncoder);
    };
}