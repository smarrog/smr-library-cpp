#include "amf3Decoder.hpp"

#include "serialization/utils.hpp"
#include "serialization/exceptions.hpp"

using namespace smr;

#define MIN_INT_29 = -268435456;
#define MAX_INT_29 = 268435455;

enum class Marker {
    UNDEFINED = 0x00,
    NULL_MARKER = 0x01,
    FALSE = 0x02,
    TRUE = 0x03,
    INTEGER = 0x04,
    DOUBLE = 0x05,
    STRING = 0x06,
    XML = 0x07,
    DATE = 0x08,
    ARRAY = 0x09,
    OBJECT = 0x0A,
    XML_END = 0x0B,
    BYTE_ARRAY = 0x0C,
    VECTOR_INT = 0x0D,
    VECTOR_UINT = 0x0E,
    VECTOR_DOUBLE = 0x0F,
    VECTOR_OBJECT = 0x10,
    DICTIONARY = 0x11
};

void checkMarker(std::istream& is, Marker marker) {
    if (static_cast<Marker>(is.peek()) != marker) {
        throw UnexpectedTokenException(is.peek());
    } else {
        is.ignore();
    }
}

Serializable Amf3Decoder::decode() {
    checkToken(_is, 0x03, "version");
    decodeHeaders();
    switch (static_cast<Marker>(_is.peek())) {
        case Marker::UNDEFINED:
            _is.ignore(1);
            return Undefined();
        case Marker::NULL_MARKER:
            _is.ignore(1);
            return nullptr;
        case Marker::FALSE:
            _is.ignore(1);
            return false;
        case Marker::TRUE:
            _is.ignore(1);
            return true;
        case Marker::INTEGER:
            return decodeInteger();
        case Marker::DOUBLE:
            return decodeDouble();
        case Marker::STRING:
            return decodeString();
        case Marker::XML:
            return decodeXML();
        case Marker::DATE:
            return decodeDate();
        case Marker::ARRAY:
            return decodeArray();
        case Marker::OBJECT:
            return decodeObject();
        case Marker::BYTE_ARRAY:
            return decodeByteArray();
        case Marker::VECTOR_INT:
            return decodeVectorInt();
        case Marker::VECTOR_UINT:
            return decodeVectorUint();
        case Marker::VECTOR_DOUBLE:
            return decodeVectorDouble();
        case Marker::VECTOR_OBJECT:
            return decodeVectorObject();
        case Marker::DICTIONARY:
            return decodeDictionary();
        default:
            throw UnexpectedTokenException(_is.peek());
    }
}

void Amf3Decoder::decodeHeaders() {
    auto count = _is.get();
    if (count <= 0) {
        throw std::runtime_error("Headers count must be positive");
    }
    while (count --) {
        std::string headerName = "header"; // read UTF
        auto next = _is.get();
        if (next == -1) {
            throw std::runtime_error("Unexpected EOF");
        }
        bool mustUnderstand = next > 0;
        _is.ignore(4); // consume header length
        auto typeMarker = _is.peek();
        if (typeMarker == -1) {
            throw std::runtime_error("Enexpected EOF");
        } else if (typeMarker == 17) {
            _is.get();
        }
        auto header = decode();
    }
}

Serializable Amf3Decoder::decodeInteger() {
    checkMarker(_is, Marker::INTEGER);
    auto value = decode29BitInt();
    return value | -(value & (1 << 28));
}

Serializable Amf3Decoder::decodeDouble() {
    checkMarker(_is, Marker::DOUBLE);
    char bytes[8];
    _is.read(bytes, 8);
    return *reinterpret_cast<double*>(&bytes);
}

Serializable Amf3Decoder::decodeString() {
    checkMarker(_is, Marker::STRING);
    throw NotImplementException("decode string");
}

Serializable Amf3Decoder::decodeXML() {
    checkMarker(_is, Marker::XML);
    throw NotImplementException("decode XML");
}

Serializable Amf3Decoder::decodeDate() {
    checkMarker(_is, Marker::DATE);
    throw NotImplementException("decode date");
}

Serializable Amf3Decoder::decodeArray() {
    checkMarker(_is, Marker::ARRAY);
    throw NotImplementException("decode array");
}

Serializable Amf3Decoder::decodeObject() {
    checkMarker(_is, Marker::OBJECT);
    throw NotImplementException("decode object");
}

Serializable Amf3Decoder::decodeByteArray() {
    checkMarker(_is, Marker::BYTE_ARRAY);
    throw NotImplementException("decode byteArray");
}

Serializable Amf3Decoder::decodeVectorInt() {
    checkMarker(_is, Marker::VECTOR_INT);
    throw NotImplementException("decode vector of int");
}

Serializable Amf3Decoder::decodeVectorUint() {
    checkMarker(_is, Marker::VECTOR_UINT);
    throw NotImplementException("decode vector of uint");
}

Serializable Amf3Decoder::decodeVectorDouble() {
    checkMarker(_is, Marker::VECTOR_DOUBLE);
    throw NotImplementException("decode vecotr of double");
}

Serializable Amf3Decoder::decodeVectorObject() {
    checkMarker(_is, Marker::VECTOR_OBJECT);
    throw NotImplementException("decode vector of objects");
}

Serializable Amf3Decoder::decodeDictionary() {
    checkMarker(_is, Marker::DICTIONARY);
    throw NotImplementException("decode dictionary");
}

// HELPERS

int Amf3Decoder::decode29BitInt() {
    auto step = 0;
    auto value = 0;
    while (_is.peek() != -1 && step-- < 4) {
        auto next = _is.get();
        if (next < 0b1000'0000) {
            return value | next & 0x7F;
        }
        value <<= 7;
    }
    throw std::runtime_error("Wrong 29bit Integer format");
}