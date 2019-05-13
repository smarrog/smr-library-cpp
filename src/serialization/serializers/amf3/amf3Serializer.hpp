#pragma once

#include "serialization/serializer.hpp"

namespace smr {
    class Amf3Serializer final : public Serializer {
    DECLARE_SERIALIZER(Amf3Serializer)
    };
}