#include "dsvSerializer.hpp"
#include "dsvDecoder.hpp"
#include "dsvEncoder.hpp"
#include "serialization/exceptions.hpp"

#include <iostream>

using namespace smr;

REGISTER_SERIALIZER(dsv, DsvSerializer)

Serializable DsvSerializer::decode(std::istream& is) {
	try {
		return DsvDecoder(is, getConfig()).decode();
	} catch(std::exception& exception) {
		throw DecodeException("dsv", is, exception);
	}
}

void DsvSerializer::encode(std::ostream& os, const Serializable& value) {
	try {
		return DsvEncoder(os, getConfig()).encode(value);
	} catch(std::exception& exception) {
		throw EncodeException("dsv", exception);
	}
}