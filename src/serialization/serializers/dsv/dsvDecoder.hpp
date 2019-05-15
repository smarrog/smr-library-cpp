#pragma once

#include "serialization/decoder.hpp"

#include "string_view"

namespace smr {
	class DsvDecoder final : public Decoder {
	public:
		explicit DsvDecoder(std::istream& is, const SerializerConfig& config)
		    : Decoder(is, config) {}

		DsvDecoder(const DsvDecoder&) = delete;
		DsvDecoder& operator = (DsvDecoder const&) = delete;
		DsvDecoder(DsvDecoder&&) noexcept = delete;
		DsvDecoder& operator = (DsvDecoder&&) noexcept = delete;

        Serializable decode();

	private:
		Array decodeLine(std::string_view sv, char separator);

		size_t _expectedSize = -1;
	};
}
