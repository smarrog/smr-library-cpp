#include "iniSerializer.hpp"
#include "iniDecoder.hpp"
#include "iniEncoder.hpp"
#include "serialization/exceptions.hpp"

#include <iostream>

using namespace smr;

REGISTER_SERIALIZER(ini, IniSerializer)

Serializable IniSerializer::decode(std::istream& is) {
    try {
        return IniDecoder(is, getConfig()).decode();
    } catch(std::exception& exception) {
        throw DecodeException("ini", is, exception);
    }
}

void IniSerializer::encode(std::ostream& os, const Serializable& value) {
    try {
        return IniEncoder(os, getConfig()).encode(value);
    } catch(std::exception& exception) {
        throw EncodeException("ini", exception);
    }
}