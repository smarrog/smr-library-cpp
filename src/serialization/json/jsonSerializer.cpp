#include "jsonSerializer.hpp"
#include "jsonDecoder.hpp"
#include "jsonEncoder.hpp"
#include "../exceptions/decodeException.hpp"
#include "../exceptions/encodeException.hpp"

#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <sstream>

using namespace smr;

REGISTER_SERIALIZER(json, JsonSerializer)

Serializable JsonSerializer::decode(std::istream& is) const {
    try {
        return JsonDecoder(is, _flags).decode();
    } catch(std::exception& exception) {
        throw DecodeException("json", is, exception);
    }
}

void JsonSerializer::encode(std::ostream& os, const Serializable& value) const {
    try {
        return JsonEncoder(os, _flags).encode(value);
    } catch(std::exception& exception) {
        throw EncodeException("json", exception);
    }
}