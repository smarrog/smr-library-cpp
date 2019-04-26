#pragma once

#include "stuff/flags.hpp"

namespace smr {
    struct SerializerConfig {
        Flags flags;
        char separator;
    };
}