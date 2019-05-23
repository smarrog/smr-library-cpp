#include "amf3Decoder.hpp"

#include "serialization/utils.hpp"
#include "serialization/exceptions.hpp"

using namespace smr;

enum class Marker {
    UNDEFINED = 0x00,
    NULL_MARKER = 0x01,
    FALSE = 0x02,
    TRUE = 0x03,
    INTEGER = 0x04,
    FLOAT = 0x05,
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

void Amf3Decoder::decodeHeaders() {
    auto count = getUintFromStream<uint16_t>(_is);
    if (count <= 0) {
        throw std::runtime_error("Headers count must be positive");
    }
    while (count--) {
        auto headerName = decodeHeadlessString();
        _is.ignore(1); // mustUnderstand
        auto next = _is.get();
        bool mustUnderstand = next > 0;
        auto headerLength = getUintFromStream<uint32_t>(_is);

        auto header = decode();
    }
}

void Amf3Decoder::decodeMessages() {
    auto count = getUintFromStream<uint16_t>(_is);
    if (count <= 0) {
        throw std::runtime_error("Messages count must be positive");
    }
    while (count-- > 0) {
        auto targetURI = decodeHeadlessString();
        auto responseURI = decodeHeadlessString();
        auto messageLength = getUintFromStream<uint32_t>(_is);
        auto messageBody = decode();
    }
}

Serializable Amf3Decoder::decode() {
    if (!_config.flags.isFlagSet(serialization::flags::HEADLESS)) {
        checkToken(_is, 0x03, "version");
        decodeHeaders();
        decodeMessages();
    }
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
        case Marker::FLOAT:
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

Serializable Amf3Decoder::decodeInteger() {
    checkMarker(_is, Marker::INTEGER);
    return decodeHeadlessInteger();
}

Serializable Amf3Decoder::decodeDouble() {
    checkMarker(_is, Marker::FLOAT);
    char bytes[8];
    _is.read(bytes, 8);
    // TODO BIG ENDIAN or LITTLE ENDIAN ?
    return *reinterpret_cast<double *>(&bytes);
}

Serializable Amf3Decoder::decodeString() {
    checkMarker(_is, Marker::STRING);
    return decodeHeadlessString();
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

    auto tmp = decodeHeadlessInteger();
    if (tmp & 0x01) { // value
        size_t length = tmp >> 1;
        Array array;

        while (true) { // Associative portion
            auto name = decodeHeadlessString();
            if (name.empty()) {
                break;
            }
            auto value = decode();
            // TODO do something with associative pair
        }

        while (length-- > 0) { // Dense portion
            array.emplace_back(decode());
        }

        return array;
    }
    throw NotImplementException("Array reference");
}

Serializable Amf3Decoder::decodeObject() {
    checkMarker(_is, Marker::OBJECT);

    Amf3Traits traits;
    uint32_t tmp = decodeHeadlessInteger();
    if ((tmp & 0b0000'0001) == 0) {
        // reference
    } else if ((tmp & 0b0000'0011) == 0b01) {
        // traits reference
        traits = _context.getTraits(tmp >> 2);
    } else if ((tmp & 0b0000'0111) == 0b111) {
        // traits external
        traits.className = decodeHeadlessString();
        traits.externalizable = true;
        _context.putTraits(traits);
    } else {
        // traits
        traits.className = decodeHeadlessString();
        if ((tmp & 0b0000'1000) != 0) {
            traits.dynamic = true;
            uint32_t sealedMembersCount = tmp >> 4;
            for (uint32_t i = 0; i < sealedMembersCount; ++i) {
                traits.addAttribute(decodeHeadlessString());
            }
        }
        _context.putTraits(traits);
    }

    Object object;

    if (traits.externalizable) {
        throw std::runtime_error("Externalizable objects are not supported");
        return object;
    }

    for (const auto& name : traits.getAttributes()) {
        object.emplace(name, decode());
    }

    if (!traits.dynamic) {
        return object;
    }

    while (true) {
        std::string key = decodeHeadlessString();
        if (key.empty()) {
            return object;
        }
        object.emplace(std::move(key), decode());
    }
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
    throw NotImplementException("decode vector of double");
}

Serializable Amf3Decoder::decodeVectorObject() {
    checkMarker(_is, Marker::VECTOR_OBJECT);
    throw NotImplementException("decode vector of objects");
}

Serializable Amf3Decoder::decodeDictionary() {
    checkMarker(_is, Marker::DICTIONARY);

    auto tmp = decodeHeadlessInteger();
    _is.ignore(); // weakness
    if (tmp & 0x01) { // value
        size_t length = tmp >> 1;
        Object object;
        while (length-- > 0) {
            object.emplace(decodeHeadlessString(), decode());
        }
        return object;
    }
    throw NotImplementException("Dictionary reference");
}

// HEADLESS

int Amf3Decoder::decodeHeadlessInteger() {
    uint32_t value = 0;
    int byte = 4;
    while (true) {
        if (_is.eof()) {
            throw UnexpectedEofException();
        }
        auto u8 = static_cast<uint8_t>(_is.get());
        if (--byte) {
            value = (value << 7) | (u8 & 0x7F);
            if ((u8 & 0x80) == 0) {
                break;
            }
        } else {
            value = (value << 8) | (u8 & 0xFF);
            break;
        }
    }
    return value;
}

std::string Amf3Decoder::decodeHeadlessString() {
    std::string str;

    auto tmp = decodeHeadlessInteger();
    if (tmp & 0x01) { // value
        size_t length = tmp >> 1;
        std::ostringstream oss;
        while (length > 0) {
            length -= utf8SymbolToStream(oss, getUtf8Symbol(_is));
        }
        str = oss.str();
    } else { // reference
        str = _context.getString(tmp >> 1);
    }

    if (!str.empty() && _context.getIndex(str) == -1) {
        _context.putString(str);
    }

    return str;
}