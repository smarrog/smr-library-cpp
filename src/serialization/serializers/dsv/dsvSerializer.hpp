#pragma once

#include "serialization/serializer.hpp"

namespace smr {
	class DsvSerializer final : public Serializer {
	DECLARE_SERIALIZER(DsvSerializer)
	};
}