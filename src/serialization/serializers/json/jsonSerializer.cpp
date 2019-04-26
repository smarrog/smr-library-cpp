#include "jsonSerializer.hpp"
#include "jsonDecoder.hpp"
#include "jsonEncoder.hpp"
#include "serialization/exceptions/decodeException.hpp"
#include "serialization/exceptions/encodeException.hpp"

#include <iostream>

using namespace smr;

REGISTER_SERIALIZER(json, JsonSerializer)

Serializable JsonSerializer::decode(std::istream& is) const {
    try {
        return JsonDecoder(is, _config).decode();
    } catch(std::exception& exception) {
        throw DecodeException("json", is, exception);
    }
}

void JsonSerializer::encode(std::ostream& os, const Serializable& value) const {
    try {
        return JsonEncoder(os, _config).encode(value);
    } catch(std::exception& exception) {
        throw EncodeException("json", exception);
    }
}