#pragma once

#include "compression/compressor.hpp"

namespace smr {
    class GzipCompressor final : public Compressor {
    DECLARE_COMPRESSOR(GzipCompressor)
    };
}
