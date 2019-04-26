#pragma once

#include "serialization/serializer.hpp"

namespace smr {
    class JsonSerializer final : public Serializer {
        DECLARE_SERIALIZER(JsonSerializer)
    };
}