#pragma once

#include "serialization/decoder.hpp"

namespace smr {
	class XmlDecoder final : public Decoder {
	public:
		explicit XmlDecoder(std::istream& is, const SerializerConfig& config)
			: Decoder(is, config) {}

		XmlDecoder(const XmlDecoder&) = delete;
		XmlDecoder& operator = (XmlDecoder const&) = delete;
		XmlDecoder(XmlDecoder&&) noexcept = delete;
		XmlDecoder& operator = (XmlDecoder&&) noexcept = delete;

		Serializable decode();
	};
}