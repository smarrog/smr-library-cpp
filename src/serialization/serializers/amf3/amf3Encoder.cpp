#include "amf3Encoder.hpp"
#include "serialization/exceptions.hpp"
#include "serialization/utils.hpp"

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

void Amf3Encoder::encode(const Serializable& serializable) {
    if (serializable.isUndefined()) {
        return encode(serializable.asUndefined());
    } else if (serializable.isNull()) {
        return encode(serializable.asNull());
    } else if (serializable.isBool()) {
        return encode(serializable.asBool());
    } else if (serializable.isInt()) {
        return encode(serializable.asInt());
    } else if (serializable.isFloat()) {
        return encode(serializable.asFloat());
    } else if (serializable.isString()) {
        return encode(serializable.asString());
    } else if (serializable.isArray()) {
        return encode(serializable.asArray());
    } else if (serializable.isObject()) {
        return encode(serializable.asObject());
    }
}

void Amf3Encoder::encode(std::nullptr_t) {
    _os << static_cast<char>(Marker::NULL_MARKER);
}

void Amf3Encoder::encode(Undefined) {
    _os << static_cast<char>(Marker::UNDEFINED);
}

void Amf3Encoder::encode(bool value) {
    _os << static_cast<char>(value ? Marker::TRUE : Marker::FALSE);
}

void Amf3Encoder::encode(int value) {
    if (value >= 1 << 29) {
        encode(static_cast<double>(value));
        return;
    }
    _os << static_cast<char>(Marker::INTEGER);
    int byte = 4;
    while (true) {
        auto u8 = static_cast<uint8_t>(value & 0x7F);
        value >>= 7;
        if (--byte && value) {
            _os.put(static_cast<char>(0x80 | u8));
        } else {
            _os.put(static_cast<char>(u8));
            break;
        }
    }
}

void Amf3Encoder::encode(double value) {
    _os << static_cast<char>(Marker::FLOAT);
    union {
        double flt;
        char byte[8];
    } data = {value};
    // TODO LITTLE ENDIAN OR BIG ENDIAN
    data.flt = value;
    for (auto i = 0u; i < sizeof(double); i++) {
        _os.put(data.byte[i]);
    }
}

void Amf3Encoder::encode(const std::string& value) {
    _os << static_cast<char>(Marker::STRING);
    encodeHeadlessString(value);
}

void Amf3Encoder::encode(const Array& value) {
    _os << static_cast<char>(Marker::ARRAY);
    encodeHeadlessInteger((value.size() << 1) | 0x01);
    encodeHeadlessString(""); // Associative portion
    for (auto& elem : value) {
        encode(elem);
    }
}

void Amf3Encoder::encode(const Object& value) {
    _os << static_cast<char>(Marker::OBJECT);
    uint32_t sealedMembersCount = 0;

    uint32_t traitMarker = sealedMembersCount << 4 | 0x03; // U29-traits = 0b0011 = 0x03
    encodeHeadlessInteger(traitMarker | 0x08); // dynamic marker = 0b1000 = 0x08
    encodeHeadlessString(""); // class name

    for (const auto& it : value) {
        encodeHeadlessString(it.first);
        encode(it.second);
    }
    encodeHeadlessString("");
}

void Amf3Encoder::encodeHeadlessInteger(uint32_t value) {
    value = static_cast<uint32_t>(value);
    if (value <= 0x0000007F) { // 0xxxxxxx
        _os.put(value & 0x7F);
    } else if (value <= 0x00003FFF) { // 1xxxxxxx 0xxxxxxx
        _os.put(((value >> 7) & 0x7F) | 0x80);
        _os.put(value & 0x7F);
    } else if (value <= 0x001FFFFF) { // 1xxxxxxx 1xxxxxxx 0xxxxxxx
        _os.put(((value >> 14) & 0x7F) | 0x80);
        _os.put(((value >> 7) & 0x7F) | 0x80);
        _os.put(value & 0x7F);
    } else if (value <= 0x3FFFFFFF) { // 1xxxxxxx 1xxxxxxx 1xxxxxxx xxxxxxxx
        _os.put(((value >> 22) & 0x7F) | 0x80);
        _os.put(((value >> 15) & 0x7F) | 0x80);
        _os.put(((value >> 8) & 0x7F) | 0x80);
        _os.put(value & 0xFF);
    } else {
        throw std::runtime_error(std::string("Can't write ") + std::to_string(value) + std::string(" to stream"));
    }
}

void Amf3Encoder::encodeHeadlessString(const std::string& str) {
    int32_t index = _context.getIndex(str);
    if (index == -1) {
        encodeHeadlessInteger((str.size() << 1) | 0x01);
        if (!str.empty()) {
            _os.write(str.data(), str.size());
            _context.putString(str);
        }
    } else {
        encodeHeadlessInteger(index << 1);
    }
}

void Amf3Encoder::encodeDictionary(const Object& value) {
    _os << static_cast<char>(Marker::DICTIONARY);

    encodeHeadlessInteger((value.size() << 1) | 0x01);
    _os.put(0x00); // Weakness
    for (auto&[prop, elem] : value) {
        encodeHeadlessString(prop);
        encode(elem);
    }
}