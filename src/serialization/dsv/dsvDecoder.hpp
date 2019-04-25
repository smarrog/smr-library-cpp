#pragma once

#include "dsvSerializer.hpp"
#include "../decoder.hpp"

#include "string_view"

namespace smr {
	class DsvDecoder final : public Decoder {
		DECLARE_DECODER(DsvSerializer, DsvDecoder)

	private:
		Array decodeLine(std::string_view sv);

		size_t _expectedSize = -1;
		const char _separator = ',';
	};
}
