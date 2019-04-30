#include "crc32Serializer.hpp"
#include "crc32Encoder.hpp"

#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <sstream>

using namespace smr;

REGISTER_SERIALIZER(crc32, Crc32Serializer)

Serializable Crc32Serializer::decode(std::istream& is) const {
    throw std::runtime_error("Crc32 can't be decoded");
}

void Crc32Serializer::encode(std::ostream& os, const Serializable& value) const {
    try {
        return Crc32Encoder(os, _config).encode(value);
    } catch(std::exception& exception) {
        throw EncodeException("crc32", exception);
    }
}