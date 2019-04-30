#pragma once

#include "serialization/serializer.hpp"
#include "serialization/exceptions.hpp"

namespace smr {
    class Crc32Serializer final : public Serializer {
    DECLARE_SERIALIZER(Crc32Serializer)
    };
}