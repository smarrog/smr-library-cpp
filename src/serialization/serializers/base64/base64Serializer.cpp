#include "base64Serializer.hpp"
#include "base64Decoder.hpp"
#include "base64Encoder.hpp"

#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <sstream>

using namespace smr;

REGISTER_SERIALIZER(base64, Base64Serializer)

Serializable Base64Serializer::decode(std::istream& is) const {
    try {
        return Base64Decoder(is, _config).decode();
    } catch(std::exception& exception) {
        throw DecodeException("base64", is, exception);
    }
}

void Base64Serializer::encode(std::ostream& os, const Serializable& value) const {
    try {
        return Base64Encoder(os, _config).encode(value);
    } catch(std::exception& exception) {
        throw EncodeException("base64", exception);
    }
}