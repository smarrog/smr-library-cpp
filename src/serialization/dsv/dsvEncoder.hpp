#pragma once

#include "dsvSerializer.hpp"
#include "../encoder.hpp"

namespace smr {
	class DsvEncoder final : public Encoder {
	public:
		explicit DsvEncoder(std::ostream& os, size_t flags)
			: Encoder(os, flags) {}

		DsvEncoder(const DsvEncoder&) = delete;
		DsvEncoder& operator = (DsvEncoder const&) = delete;
		DsvEncoder(DsvEncoder&&) noexcept = delete;
		DsvEncoder& operator = (DsvEncoder&&) noexcept = delete;

        void encode(const Serializable& serializable, char separator);
	};
}