#include "xmlSerializer.hpp"
#include "xmlDecoder.hpp"
#include "xmlEncoder.hpp"
#include "serialization/exceptions.hpp"

#include <iostream>

using namespace smr;

REGISTER_SERIALIZER(xml, XmlSerializer)

Serializable XmlSerializer::decode(std::istream& is) const {
	try {
		return XmlDecoder(is, _config).decode();
	} catch(std::exception& exception) {
		throw DecodeException("xml", is, exception);
	}
}

void XmlSerializer::encode(std::ostream& os, const Serializable& value) const {
	try {
		return XmlEncoder(os, _config).encode(value);
	} catch(std::exception& exception) {
		throw EncodeException("xml", exception);
	}
}