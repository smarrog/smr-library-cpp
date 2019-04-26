#pragma once

#include "serialization/serializer.hpp"

namespace smr {
    class IniSerializer final : public Serializer {
    DECLARE_SERIALIZER(IniSerializer)
    };
}