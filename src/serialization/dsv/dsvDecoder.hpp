#pragma once

#include "dsvSerializer.hpp"
#include "../decoder.hpp"

#include "string_view"

namespace smr {
	class DsvDecoder final : public Decoder {
	public:
		explicit DsvDecoder(std::istream& is, size_t flags)
		: Decoder(is, flags) {}
		DsvDecoder(const DsvDecoder&) = delete;
		DsvDecoder& operator = (DsvDecoder const&) = delete;
		DsvDecoder(DsvDecoder&&) noexcept = delete;
		DsvDecoder& operator = (DsvDecoder&&) noexcept = delete;

        Serializable decode(char separator);

	private:
		Array decodeLine(std::string_view sv, char separator);

		size_t _expectedSize = -1;
	};
}
