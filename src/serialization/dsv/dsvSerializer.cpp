#include "dsvSerializer.hpp"
#include "dsvDecoder.hpp"
#include "dsvEncoder.hpp"
#include "../exceptions/decodeException.hpp"
#include "../exceptions/encodeException.hpp"

#include <iostream>

using namespace smr;

REGISTER_SERIALIZER(dsv, DsvSerializer)

Serializable DsvSerializer::decode(std::istream& is) const {
	try {
		return DsvDecoder(is, _config).decode();
	} catch(std::exception& exception) {
		throw DecodeException("dsv", is, exception);
	}
}

void DsvSerializer::encode(std::ostream& os, const Serializable& value) const {
	try {
		return DsvEncoder(os, _config).encode(value);
	} catch(std::exception& exception) {
		throw EncodeException("dsv", exception);
	}
}