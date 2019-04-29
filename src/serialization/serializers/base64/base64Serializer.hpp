#pragma once

#include "serialization/serializer.hpp"
#include "serialization/exceptions.hpp"

namespace smr {
    class Base64Serializer final : public Serializer {
    DECLARE_SERIALIZER(Base64Serializer)
    };
}