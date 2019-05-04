#pragma once

#include "serialization/serializer.hpp"

namespace smr {
	class XmlSerializer final : public Serializer {
	DECLARE_SERIALIZER(XmlSerializer)
	};
}