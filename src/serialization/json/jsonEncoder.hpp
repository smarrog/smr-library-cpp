#pragma once

#include "jsonSerializer.hpp"
#include "../encoder.hpp"

namespace smr {
    class JsonEncoder final : public Encoder {
    DECLARE_ENCODER(JsonSerializer, JsonEncoder)

    private:
        size_t _offset = 0;

        inline void nextLine();
        inline void addSpacing();

        template <typename Container, typename ElemEncoder>
        void encodeContainer(const Container& container, char open, char close, ElemEncoder elemEncoder);
    };
}