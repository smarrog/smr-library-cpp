#include "iniSerializer.hpp"
#include "iniDecoder.hpp"
#include "iniEncoder.hpp"
#include "serialization/exceptions.hpp"

#include <iostream>

using namespace smr;

REGISTER_SERIALIZER(ini, IniSerializer)

Serializable IniSerializer::decode(std::istream& is) const {
    try {
        return IniDecoder(is, _config).decode();
    } catch(std::exception& exception) {
        throw DecodeException("ini", is, exception);
    }
}

void IniSerializer::encode(std::ostream& os, const Serializable& value) const {
    try {
        return IniEncoder(os, _config).encode(value);
    } catch(std::exception& exception) {
        throw EncodeException("ini", exception);
    }
}