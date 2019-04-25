#pragma once

#include "dsvSerializer.hpp"
#include "../encoder.hpp"

namespace smr {
	class DsvEncoder final : public Encoder {
		DECLARE_ENCODER(DsvSerializer, DsvEncoder)

	private:
		const char _separator = ',';
	};
}