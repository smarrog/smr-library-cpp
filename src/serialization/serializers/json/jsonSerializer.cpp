#include "jsonSerializer.hpp"
#include "jsonDecoder.hpp"
#include "jsonEncoder.hpp"
#include "serialization/exceptions.hpp"

#include <iostream>

using namespace smr;

REGISTER_SERIALIZER(json, JsonSerializer)

Serializable JsonSerializer::decode(std::istream& is) {
    try {
        return JsonDecoder(is, getConfig()).decode();
    } catch(std::exception& exception) {
        throw DecodeException("json", is, exception);
    }
}

void JsonSerializer::encode(std::ostream& os, const Serializable& value) {
    try {
        return JsonEncoder(os, getConfig()).encode(value);
    } catch(std::exception& exception) {
        throw EncodeException("json", exception);
    }
}