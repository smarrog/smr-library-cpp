#pragma once

#include <stdint.h>
#include <string>

namespace smr {
    class Crc32 {
    public:
        static uint32_t hash(const std::string& s);
    };
}